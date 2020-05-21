/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2009
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#include <iostream>
#include <string>
#include <sstream>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "cimage.h"
#include "texception.h"

extern "C" {
  #include "jpeglib.h"
}

#define DCT_IDCT
#include "platform.h"

using namespace std;

namespace bfs = boost::filesystem;

/** Construct image object - load the image from the given file.
    @param filename_in File (path) of the JPEG image to be loaded.
  */
cimage::cimage(std::string filename_in){
  /* Initialize the JPEG decompression object with default error handling. */
  srcinfo.err = jpeg_std_error(&jsrcerr);
  jpeg_create_decompress(&srcinfo);
  /* Open the input file. */
  const char* filename = filename_in.c_str();
  if ((input_file = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "Can't open %s\n", filename_in.c_str());
  }
  /* Specify data source for decompression */
  jpeg_stdio_src(&srcinfo, input_file);
  /* Read file header */
  (void) jpeg_read_header(&srcinfo,TRUE);
  /* Read the DCT coefficients of the source file */
  src_coef_arrays = jpeg_read_coefficients(&srcinfo);
  /* Assign some simplifying pinters */
  QuantizationTable = (*(((srcinfo).quant_tbl_ptrs)[0])).quantval;
  DCTs = *(*(((*(*src_coef_arrays))).mem_buffer));/* I love these image structures */
  NumberOfDCTBlocks.width = srcinfo.comp_info->width_in_blocks;
  NumberOfDCTBlocks.height = srcinfo.comp_info->height_in_blocks;
  NumberOfCroppedDCTBlocks.width=NumberOfDCTBlocks.width-1;
  NumberOfCroppedDCTBlocks.height=NumberOfDCTBlocks.height-1;
  ImgSize.width = NumberOfDCTBlocks.width*8;
  ImgSize.height = NumberOfDCTBlocks.height*8;
  CroppedImgSize.width = ImgSize.width-8;
  CroppedImgSize.height = ImgSize.height-8;
  Pixels = new Ipp8u[ImgSize.height*ImgSize.width];
  DCTs_row = new Ipp16s[ImgSize.height*ImgSize.width];
  CroppedPixels = new Ipp8u[(NumberOfDCTBlocks.width-1)*(NumberOfDCTBlocks.height-1)*64];
  CroppedDCTs = new Ipp16s[(NumberOfDCTBlocks.width-1)*(NumberOfDCTBlocks.height-1)*64];
  Pixels_row = new Ipp8u[NumberOfDCTBlocks.width*NumberOfDCTBlocks.height*64];
  CroppedPixels_row = new Ipp8u[(NumberOfDCTBlocks.width-1)*(NumberOfDCTBlocks.height-1)*64];
  CroppedDCTs_row = new Ipp16s[NumberOfCroppedDCTBlocks.width*NumberOfCroppedDCTBlocks.height*64];

  featurevector = new cfeaturevector;
  bfs::path full_path( filename_in );
  imagename = full_path.leaf();
  NumberOfColorComponents = srcinfo.num_components;
  AlreadyReleased = false;
  MsgTooLong = false;
}
  /** Destructor of the class cimage. */
cimage::~cimage(){
	if (!AlreadyReleased)
		Release();
}
  /** Release the image without saving. */
void cimage::Release(){
  (void) jpeg_finish_decompress(&srcinfo);
  jpeg_destroy_decompress(&srcinfo);
  /* Close the input file, if we opened it */
  if (input_file != stdin) fclose(input_file);
  /* release all allocated memory */
  delete[] DCTs_row;
  delete[] Pixels;
  delete[] Pixels_row;
  delete[] CroppedPixels;
  delete[] CroppedPixels_row;
  delete[] CroppedDCTs;
  delete[] CroppedDCTs_row;
  delete featurevector;
  AlreadyReleased = true;
}

/**
 * Decompress DCT blocks into the spatial domain using source and target.
 *
 * @param source Pointer to the source field of DCT coefficients (8x8 block structure)
 * @param target Pointer to the target field of pixels (8x8 block structure)
 * @param size Number of DCT blocks in the source field (width and height of the field)
 * @param QTable Quantization table being considered
 * @param compressor What DCT implementation should be used
 */
void cimage::DCT2Spatial(Ipp16s* source,Ipp8u* target,IppiSize size, Ipp16u* QTable,char* compressor){
	/* decompress the image block by block */
	for (int i=0;i<size.height;i++)
		for (int j=0;j<size.width;j++)
			iDCT_8x8(&source[(i*size.width+j)*64],&target[(i*size.width+j)*64],QTable,compressor);
}
/**
 * Compress spatial image into DCT blocks using source and target.
 *
 * @param source Pointer to the source field of pixel values (8x8 block structure)
 * @param target Pointer to the target field of DCT coefficients (8x8 block structure)
 * @param size Number of DCT blocks in the target field (width and height of the field)
 * @param QTable Quantization table being considered
 * @param compressor What DCT implementation should be used
 */
void cimage::Spatial2DCT(Ipp8u* source,Ipp16s* target, IppiSize size, Ipp16u* QTable,char* compressor){
	/* compress the image block by block */

    for (int i=0;i<size.height;i++)
      for (int j=0;j<size.width;j++)
        DCT_8x8(&source[(i*size.width+j)*64],&target[(i*size.width+j)*64],QTable,compressor);
}

/**
 * Transform the image from 8x8 block format to the 'row by row' format - Ipp8u variant.
 *
 * @param source Pointer to the source image in 8x8 block format
 * @param target Pointer to the target image
 * @param size Number of 8x8 blocks in the source field (width and height of the field)
 */
void cimage::Block2Row(Ipp8u* source, Ipp8u* target, IppiSize size){
//	cout << "Width: " << size.width << ", Height: " << size.height << endl;
	for (int i=0;i<size.height*8;i++){
		for (int j=0;j<size.width*8;j++){
			target[i*size.width*8+j]=source[(i/8)*size.width*64+(j/8)*64+(i%8)*8+(j%8)];
		}
	}
		
}
/**
 * Transform the image from 8x8 block format to the 'row by row' format - Ipp16s variant.
 *
 * @param source Pointer to the source image in 8x8 block format
 * @param target Pointer to the target image
 * @param size Number of 8x8 blocks in the source field (width and height of the field)
 */
void cimage::Block2Row(Ipp16s* source, Ipp16s* target, IppiSize size){
	for (int i=0;i<size.height*8;i++){
		for (int j=0;j<size.width*8;j++){
			target[i*size.width*8+j]=source[(i/8)*size.width*64+(j/8)*64+(i%8)*8+(j%8)];//LEAK2
		}
	}
		
}
/**
 * Transform the image from the 'row by row' to the 8x8 block format - Ipp8u variant.
 *
 * @param source Pointer to the source image in the 'row by row'
 * @param target Pointer to the target image
 * @param size Number of 8x8 blocks in the target field (width and height of the field)
 */
void cimage::Row2Block(Ipp8u* source, Ipp8u* target, IppiSize size){
	for (int i=0;i<size.height*8;i++){
		for (int j=0;j<size.width*8;j++){
			target[(i/8)*size.width*64+(j/8)*64+(i%8)*8+(j%8)]=source[i*size.width*8+j];
		}
	}

}
/**
 * Transform the image from the 'row by row' to the 8x8 block format - Ipp16s variant.
 *
 * @param source Pointer to the source image in the 'row by row'
 * @param target Pointer to the target image
 * @param size Number of 8x8 blocks in the target field (width and height of the field)
 */
void cimage::Row2Block(Ipp16s* source, Ipp16s* target, IppiSize size){
	for (int i=0;i<size.height*8;i++){
		for (int j=0;j<size.width*8;j++){
			target[(i/8)*size.width*64+(j/8)*64+(i%8)*8+(j%8)]=source[i*size.width*8+j];
		}
	}

}
/**
 * Converts block coordinates [i,j,k] into the corresponding index in the row representation.
 *
 * @param i 1st block coordinate
 * @param j 2nd block coordinate
 * @param k 3rd block coordinate
 * @param size Size of the block structure
 * @return the index (in the row representation) corresponding to the block coordinates [i,j,k]
 */
Ipp32s cimage::IndexBlock2Row(int i, int j, int k, IppiSize size){
	return (i*8+k/8)*size.width*8+j*8+k-k/8*8;// :-)
}

/**
 * Crop the image.
 *
 * @param source Pointer to the source image (row by row format)
 * @param target Pointer to the image being created by cropping (row by row format)
 * @param size Number of 8x8 blocks in the original image (width and height of the field)
 * @param crophow_v What vertical cropping should be used?
 * @param crophow_h What horizontal cropping should be used?
 */
void cimage::Crop(Ipp8u* source,Ipp8u* target,IppiSize size,int crophow_v, int crophow_h){
	int counter = 0;
	for (int i=crophow_v;i<size.height*8+crophow_v-8;i++){
		for (int j=crophow_h;j<size.width*8+crophow_h-8;j++){
			target[counter]=source[i*size.width*8+j];
			counter++;
		}
	}
}

/**
 * compute desired features given appropriate quantization tables
 * 
 * THE ONLY REAL METHOD
 *
 * @param FeatureSet What kind of features should be extracted - "merged" or "double"
 * @param QuantTable1 First quantization table (in case of "merged", "double")
 * @param QuantTable2 Second quantization table (in case of "double")
 * @param compressor What DCT implementation should be used ("matlab" or "ipp")
 * @param crophow_v Vertical amount of pixels to be cropped during calibration
 * @param crophow_h Horizontal amount of pixels to be cropped during calibration
 */
void cimage::ComputeFeatures(char* compressor, char *FeatureSet, Ipp16u *QuantTable1, Ipp16u *QuantTable2, int crophow_v, int crophow_h){

  //check the compressor validity
  if (strcmp(compressor,"matlab")!=0 && strcmp(compressor,"ipp")!=0){
    throw TException("unknown compressor");
  }
  //clean the featurevector 
  featurevector->Clean();
	//DCT domain -> spatial domain - original size
	DCT2Spatial(DCTs,Pixels,NumberOfDCTBlocks,QuantTable1,compressor);
	//spatial block format -> row format - original size
	Block2Row(Pixels,Pixels_row,NumberOfDCTBlocks);
	//crop by the given number of pixels
	Crop(Pixels_row,CroppedPixels_row,NumberOfDCTBlocks,crophow_v,crophow_h);
	//spatial row format -> block format - cropped image
	Row2Block(CroppedPixels_row,CroppedPixels,NumberOfCroppedDCTBlocks);

  if (strcmp(FeatureSet,"merged")==0){
    //spatial domain -> DCT domain (using QuantTable1) - cropped image
    Spatial2DCT(CroppedPixels,CroppedDCTs,NumberOfCroppedDCTBlocks,QuantTable1,compressor);
    //DCT domain -> spatial domain (using QuantTable1) - cropped image
    DCT2Spatial(CroppedDCTs,CroppedPixels,NumberOfCroppedDCTBlocks,QuantTable1,compressor);
  } else if (strcmp(FeatureSet,"double")==0){
		//spatial domain -> DCT domain (using QuantTable2)
		Spatial2DCT(CroppedPixels,CroppedDCTs,NumberOfCroppedDCTBlocks,QuantTable2,compressor);
		//DCT domain -> spatial domain (using QuantTable2)
		DCT2Spatial(CroppedDCTs,CroppedPixels,NumberOfCroppedDCTBlocks,QuantTable2,compressor);
    //spatial domain -> DCT domain (using QuantTable1) - cropped image
    Spatial2DCT(CroppedPixels,CroppedDCTs,NumberOfCroppedDCTBlocks,QuantTable1,compressor);
    //DCT domain -> spatial domain (using QuantTable1) - cropped image
    DCT2Spatial(CroppedDCTs,CroppedPixels,NumberOfCroppedDCTBlocks,QuantTable1,compressor);
  } else {
    throw TException("unknown desired featureset");
  }

	//need for easier markov feature computation
	Block2Row(DCTs,DCTs_row,NumberOfDCTBlocks);
	Block2Row(CroppedDCTs,CroppedDCTs_row,NumberOfCroppedDCTBlocks);

	//DCT coefficients scan of the original image
	//linear in # of DCT coefficients
	for (int i=0;i<NumberOfDCTBlocks.height;i++)
		for (int j=0;j<NumberOfDCTBlocks.width;j++)
			for (int k=0;k<64;k++)
				LocalCorrectionOfFeatureVector(0,i,j,k,NumberOfDCTBlocks,ImgSize,DCTs,DCTs_row,Pixels,"plus","all");	

	//DCT coefficients scan of the cropped image
	//linear in # of DCT coefficients
	for (int i=0;i<NumberOfCroppedDCTBlocks.height;i++)
		for (int j=0;j<NumberOfCroppedDCTBlocks.width;j++)
			for (int k=0;k<64;k++)
				LocalCorrectionOfFeatureVector(1,i,j,k,NumberOfCroppedDCTBlocks,CroppedImgSize,CroppedDCTs,CroppedDCTs_row,CroppedPixels,"plus","all");

//assign normalization factors to the individual features
//linear in # of features

	//global histogram normalization
	for (int i=0;i<11;i++){
		featurevector->feature[i]->factor[0]=ImgSize.width*ImgSize.height;
		featurevector->feature[i]->factor[1]=CroppedImgSize.width*CroppedImgSize.height;
	}
	//local histogram normalization
	for (int i=11;i<66;i++){
		featurevector->feature[i]->factor[0]=NumberOfDCTBlocks.width*NumberOfDCTBlocks.height;
		featurevector->feature[i]->factor[1]=NumberOfCroppedDCTBlocks.width*NumberOfCroppedDCTBlocks.height;
	}
	// additional local histogram normalization
  // DATE: 03-17-2008
	for (int i=274;i<318;i++){
		featurevector->feature[i]->factor[0]=NumberOfDCTBlocks.width*NumberOfDCTBlocks.height;
		featurevector->feature[i]->factor[1]=NumberOfCroppedDCTBlocks.width*NumberOfCroppedDCTBlocks.height;
	}
	//dual histogram normalization
	for (int i=66;i<157;i=i+9){
		for (int j=0;j<9;j++){
			featurevector->feature[i+j]->factor[0]=featurevector->feature[(i-66)/9]->count[0];
			featurevector->feature[i+j]->factor[1]=featurevector->feature[(i-66)/9]->count[1];
		}
	}
	//variation normalization
  //////////////////////////////////////////////////////////////////////////////
  //05-02-2007: modification in order to correspond with matlab merging strategy
  //OLD VERSION:
	//featurevector->feature[165]->factor[0]=(NumberOfDCTBlocks.width*(NumberOfDCTBlocks.height-1)+(NumberOfDCTBlocks.width-1)*NumberOfDCTBlocks.height)*64;
	//featurevector->feature[165]->factor[1]=(NumberOfCroppedDCTBlocks.width*(NumberOfCroppedDCTBlocks.height-1)+(NumberOfCroppedDCTBlocks.width-1)*NumberOfCroppedDCTBlocks.height)*64;
  //NEW VERSION:
  //horizontal part
	featurevector->variation[0]->factor[0]=(NumberOfDCTBlocks.width-1)*NumberOfDCTBlocks.height*64;
	featurevector->variation[0]->factor[1]=(NumberOfCroppedDCTBlocks.width-1)*NumberOfCroppedDCTBlocks.height*64;
  //vertical part
	featurevector->variation[1]->factor[0]=NumberOfDCTBlocks.width*(NumberOfDCTBlocks.height-1)*64;
	featurevector->variation[1]->factor[1]=NumberOfCroppedDCTBlocks.width*(NumberOfCroppedDCTBlocks.height-1)*64;
  //05-02-2007: end of modification
  //////////////////////////////////////////////////////////////////////////////

  //blockiness normalization
	featurevector->feature[166]->factor[0]=(NumberOfDCTBlocks.width*(NumberOfDCTBlocks.height-1)+(NumberOfDCTBlocks.width-1)*NumberOfDCTBlocks.height)*8;
	featurevector->feature[166]->factor[1]=(NumberOfCroppedDCTBlocks.width*(NumberOfCroppedDCTBlocks.height-1)+(NumberOfCroppedDCTBlocks.width-1)*NumberOfCroppedDCTBlocks.height)*8;
	featurevector->feature[167]->factor[0]=featurevector->feature[166]->factor[0];
	featurevector->feature[167]->factor[1]=featurevector->feature[166]->factor[1];

  //cooccurence normalization

  //////////////////////////////////////////////////////////////////////////////
  //05-02-2007: modification in order to correspond with matlab merging strategy
  //OLD VERSION:
	//for (int i=168;i<193;i++){
	//	featurevector->feature[i]->factor[0]=((NumberOfDCTBlocks.height-1)*NumberOfDCTBlocks.width+NumberOfDCTBlocks.height*(NumberOfDCTBlocks.width-1))*63;
	//	featurevector->feature[i]->factor[1]=((NumberOfCroppedDCTBlocks.height-1)*NumberOfCroppedDCTBlocks.width+NumberOfCroppedDCTBlocks.height*(NumberOfCroppedDCTBlocks.width-1))*63;
	//}
  //NEW VERSION:
	for (int i=0;i<25;i++){
    //horizontal part
		featurevector->cooccurence[i]->factor[0]=NumberOfDCTBlocks.height*(NumberOfDCTBlocks.width-1)*63;
		featurevector->cooccurence[i]->factor[1]=NumberOfCroppedDCTBlocks.height*(NumberOfCroppedDCTBlocks.width-1)*63;
    //vertical part
		featurevector->cooccurence[25+i]->factor[0]=(NumberOfDCTBlocks.height-1)*NumberOfDCTBlocks.width*63;
		featurevector->cooccurence[25+i]->factor[1]=(NumberOfCroppedDCTBlocks.height-1)*NumberOfCroppedDCTBlocks.width*63;
	}

  //05-02-2007: end of modification
  //////////////////////////////////////////////////////////////////////////////

	//markov normalization - done dynamically during the scan of DCT coefficients

	//calibration of the whole feature vector
	featurevector->calibration();
}
/**
 * Export features into the given file.
 *
 * @param filename Export file path
 * @param featureset What featureset should be exported ("274" or "318"-additional local histograms)
 * @param precision Precision of the features
 * @param calitype Calibration type (1=merged,2=cc-merged)
 */
void cimage::ExportFeatures(string filename, string featureset, int precision, int calitype){
  FILE * output_file;
  output_file = fopen(filename.c_str(), "a");
  if (calitype==1){
    //default value = 274 (Merged features)
    int export_to=274;
    if (featureset=="318"){
      //additional local histograms included
      export_to=318;
    }
    for (int i=0;i<export_to;i++){
      ostringstream stm;
      stm << precision;
      fprintf(output_file,("%+." + stm.str() + "e ").c_str(),featurevector->feature[i]->value); //Tomas's format
      //fprintf(output_file,"%+1.6e ",featurevector->feature[i]->value); //my format
    }
    fprintf(output_file,imagename.c_str());
    fprintf(output_file,"\n");
    fclose(output_file);
  }
  if (calitype==2){
    ostringstream stm;
    stm << precision;
    vd ccmerged = featurevector->getCartesianCalibratedFeatures();
    for (int i=0;i<sz(ccmerged);i++){
      fprintf(output_file,("%+." + stm.str() + "e ").c_str(),ccmerged[i]);
    }
    fprintf(output_file,imagename.c_str());
    fprintf(output_file,"\n");
    fclose(output_file);
  }
}

/**
 * Forward DCT transform of 8x8 block, including -128 level shift and quantization.
 *
 * @param source Pointer to the source 8x8 block
 * @param target Pointer to the target 8x8 block
 * @param QTable Quantization table
 * @param compressor What DCT transform implementation should be used - "ipp","matlab"
 */
void cimage::DCT_8x8(Ipp8u* source,Ipp16s* target, Ipp16u* QTable,char* compressor){
  if (strcmp(compressor,"ipp")==0){
    throw TException("ipp compressor in this version disabled");
    //IPP temporarily disabled:

    /*

	  Ipp32f* temp = new Ipp32f[64];
	  //level shift -128
	  for (int k=0;k<64;k++)
		  temp[k]=(Ipp32f)(source[k]-128);
    //forward DCT transform - using IPP implementation
	  ippiDCT8x8Fwd_32f_C1I(temp);
	  //quantization loop
	  for (int k=0;k<64;k++){
		  if (temp[k]<0)//LEAK2
			  target[k]=(Ipp16s)(temp[k]/QTable[k]-0.5);
		  else
			  target[k]=(Ipp16s)(temp[k]/QTable[k]+0.5);
	  }
   	delete[] temp;

    */

  } else if (strcmp(compressor,"matlab")==0){
    double out[64];
    double in[64];
    //level shift -128
    for (int k=0;k<64;k++)
      in[k]=source[k]-128;
    //forward DCT transform - using C implementation that we use in matlab
    jpeg_fdct_islow_matlab(out,in);
    //quantization
    for (int k=0;k<64;k++){
      if (out[k]>=0)
        target[k]=(int)(out[k]/(QTable[k]*8)+0.5);
      else 
        target[k]=(int)(out[k]/(QTable[k]*8)-0.5);
    }
  }
}

/**
 * Inverse DCT transform of 8x8 block, including +128 level shift and dequantization.
 *
 * @param source Pointer to the source 8x8 block
 * @param target Pointer to the target 8x8 block
 * @param QTable Quantization table
 * @param compressor What DCT transform implementation should be used - "ipp","matlab"
 */
void cimage::iDCT_8x8(Ipp16s* source,Ipp8u* target, Ipp16u* QTable,char* compressor){
	//TODO: allocation at the beginning => make it quicker
  if (strcmp(compressor,"ipp")==0){
    throw TException("ipp compressor in this version disabled");

    //IPP temporarily disabled:

    /*
    
    Ipp32f* QuantizedSource = new Ipp32f[64];
	  Ipp32f* DCTtarget = new Ipp32f[64];
	  //dequantization
	  for (int k=0;k<64;k++)
		  QuantizedSource[k]=(Ipp32f)((float)source[k]*QTable[k]);//LEAK2
	  //inverse DCT transform - using IPP implementation
	  ippiDCT8x8Inv_32f_C1(QuantizedSource,DCTtarget);
	  //level shift +128,rounding
	  for (int k=0;k<64;k++){
		  if (DCTtarget[k]>=127.5)//LEAK2
			  target[k]=255;
		  else if (DCTtarget[k]<=-129.5)//LEAK2
			  target[k]=0;
		  else
			  target[k]=(Ipp8u)(DCTtarget[k]+128.5);
	  }
	  delete[] QuantizedSource;
	  delete[] DCTtarget;

    */
  } else if (strcmp(compressor,"matlab")==0){
    double out[64];
    double coef_block[64];
    double qt[64];
    for (int k=0;k<64;k++){
      coef_block[k]=source[k];
      qt[k]=QTable[k];
    }
    //inverse DCT transform - using C implementation that we use in matlab
    jpeg_idct_islow_matlab(out,coef_block,qt);
    //copy to the target structure
    for (int k=0;k<64;k++)
      target[k]=(Ipp8u)out[k];
  }
}
/**
 * Method responsible for all the feature vector computations.
 *
 * @param OC_switch 0-original image, 1-cropped image
 * @param i 1st block coordinate of the coefficient being considered
 * @param j 2nd block coordinate of the coefficient being considered
 * @param k 3rd block coordinate of the coefficient being considered
 * @param NumberOfBlocks Size of the block structure we operate in
 * @param ImgSize Size of the given image
 * @param DCTs Pointer to the field of DCT coefficients (block by block structure)
 * @param DCTs_row Pointer to the field of DCT coefficients (row by row structure)
 * @param Pixels Pointer to the field of pixel values - spatial domain (block by block structure)
 * @param operation 'plus'/'minus' - we are adding/subtracting the given coefficient to/from the feature vector
 * @param what_features 'all'/'dct_only'/'spatial_only' - select, which features should be added (or subtracted)
 */
void cimage::LocalCorrectionOfFeatureVector(int OC_switch, int i, int j, int k, IppiSize NumberOfBlocks, IppiSize ImgSize, Ipp16s* DCTs, Ipp16s* DCTs_row, Ipp8u* Pixels, char* operation, char* what_features){
	int sgn;
	if (strcmp(operation,"plus")==0)
		sgn=1;
	if (strcmp(operation,"minus")==0)
		sgn=-1;
	//actual indexes, in both block and row representations
	int index=(i*NumberOfBlocks.width+j)*64+k;
	int rowindex=IndexBlock2Row(i,j,k,NumberOfBlocks);
	if (strcmp(what_features,"spatial_only")!=0){
		//cout << abs(DCTs[index]);
		if (abs(DCTs[index])<6){
			//global histogram
			featurevector->feature[DCTs[index]+5]->count[OC_switch]+=sgn;
			if (k==8){
				//local histogram mode 8 (in matlab 2)
				featurevector->feature[DCTs[index]+16]->count[OC_switch]+=sgn;
				//dual histogram mode 8 (in matlab 2)
				featurevector->feature[9*(DCTs[index]+5)+66]->count[OC_switch]+=sgn;
			}
			if (k==16) {
				//local histogram mode 16 (in matlab 3)
				featurevector->feature[DCTs[index]+27]->count[OC_switch]+=sgn;
				//dual histogram mode 16 (in matlab 3)
				featurevector->feature[9*(DCTs[index]+5)+68]->count[OC_switch]+=sgn;
			}
			if (k==1) {
				//local histogram mode 1 (in matlab 9)
				featurevector->feature[DCTs[index]+38]->count[OC_switch]+=sgn;
				//dual histogram mode 1 (in matlab 9)
				featurevector->feature[9*(DCTs[index]+5)+67]->count[OC_switch]+=sgn;
			}
			if (k==9) {
				//local histogram mode 9 (in matlab 10)
				featurevector->feature[DCTs[index]+49]->count[OC_switch]+=sgn;
				//dual histogram mode 9 (in matlab 10)
				featurevector->feature[9*(DCTs[index]+5)+69]->count[OC_switch]+=sgn;
			}
			if (k==2) {
				//local histogram mode 2 (in matlab 17)
				featurevector->feature[DCTs[index]+60]->count[OC_switch]+=sgn;
				//dual histogram mode 2 (in matlab 17)
				featurevector->feature[9*(DCTs[index]+5)+70]->count[OC_switch]+=sgn;
			}
      if (k==24){
        //additional local histogram mode 2 (in matlab 17)
        featurevector->feature[DCTs[index]+312]->count[OC_switch]+=sgn;
			  //dual histogram mode 24 (in matlab 4)
			  featurevector->feature[9*(DCTs[index]+5)+71]->count[OC_switch]+=sgn;
      }
      if (k==17){
        //additional local histogram mode 17 (in matlab 11)
        featurevector->feature[DCTs[index]+301]->count[OC_switch]+=sgn;
        //dual histogram mode 17 (in matlab 11)
			  featurevector->feature[9*(DCTs[index]+5)+72]->count[OC_switch]+=sgn;
      }
      if (k==10){
        //additional local histogram mode 10 (in matlab 18)
        featurevector->feature[DCTs[index]+290]->count[OC_switch]+=sgn;
			  //dual histogram mode 10 (in matlab 18)
			  featurevector->feature[9*(DCTs[index]+5)+73]->count[OC_switch]+=sgn;
      }
      if (k==3){
        //additional local histogram mode 3 (in matlab 25)
        featurevector->feature[DCTs[index]+279]->count[OC_switch]+=sgn;
			  //dual histogram mode 3 (in matlab 25)
			  featurevector->feature[9*(DCTs[index]+5)+74]->count[OC_switch]+=sgn;
      }
		}//if (abs(DCTs[index])<6)
		//horizontal variation
		if (j<NumberOfBlocks.width-1) featurevector->variation[0]->count[OC_switch]+=sgn*abs(DCTs[index]-DCTs[index+64]);
		//vertical variation
		if (i<NumberOfBlocks.height-1) featurevector->variation[1]->count[OC_switch]+=sgn*abs(DCTs[index]-DCTs[index+64*NumberOfBlocks.width]);
	}//if (what_features!="spatial_only"){
	if (what_features!="dct_only"){
		//blockiness
		if ((i<NumberOfBlocks.height-1)&&(k>55)){
			//horizontal L1 blockiness - spatial domain
			featurevector->feature[166]->count[OC_switch]+=sgn*abs(Pixels[index]-Pixels[index+64*NumberOfBlocks.width-56]);
			//horizontal L2 blockiness - spatial domain
			featurevector->feature[167]->count[OC_switch]+=sgn*(Pixels[index]-Pixels[index+64*NumberOfBlocks.width-56])*(Pixels[index]-Pixels[index+64*NumberOfBlocks.width-56]);
		}
		if ((j<NumberOfBlocks.width-1)&&((k+1-(k+1)/8*8)==0)){
			//vertical L1 blockiness - spatial domain
			featurevector->feature[166]->count[OC_switch]+=sgn*abs(Pixels[index]-Pixels[index+57]);
			//vertical L2 blockiness - spatial domain
			featurevector->feature[167]->count[OC_switch]+=sgn*(Pixels[index]-Pixels[index+57])*(Pixels[index]-Pixels[index+57]);
		}
	}//if (what_features!="dct_only"){
	if (what_features!="spatial_only"){
		//cooccurence
		if((abs(DCTs[index])<3)&&(k>0)){
			//vertical cooccurence
			if (i<NumberOfBlocks.height-1)
				if (abs(DCTs[index+64*NumberOfBlocks.width])<3)
					featurevector->cooccurence[25+(DCTs[index+64*NumberOfBlocks.width]+2)*5+DCTs[index]+2]->count[OC_switch]+=sgn;
			//horizontal cooccurence
			if (j<NumberOfBlocks.width-1)
				if (abs(DCTs[index+64])<3)
					featurevector->cooccurence[(DCTs[index+64]+2)*5+DCTs[index]+2]->count[OC_switch]+=sgn;
		}//if((abs(DCTs[index])<3)&&(k>0))

		//MARKOV FEATURES
		bool m_horizontal = !((j==NumberOfBlocks.width-1)&&(((k+1-(k+1)/8*8)==0)||((k+2-(k+2)/8*8)==0)));
		bool m_vertical = !((i==NumberOfBlocks.height-1)&&(k>47));
		bool m_diagonal = m_horizontal && m_vertical;
		bool m_semidiagonal = (!((i==0)&&(k<16))) && m_horizontal;

  
    ///////////////////////////////////////////////////////////////
    //////// NEW VERSION OF MARKOV FEATURES
    //////// DATE: 05-02-2007

		int h1,h2;
		//horizontal markov features 
		if (m_horizontal){
			h1=abs(DCTs_row[rowindex])-abs(DCTs_row[rowindex+1]);
			h2=abs(DCTs_row[rowindex+1])-abs(DCTs_row[rowindex+2]);
			if (h1<-4)h1=-4;//LEAK
			else if (h1>4) h1=4;//LEAK
			if (h2<-4) h2=-4;//LEAK
			else if (h2>4) h2=4;//LEAK
			featurevector->markov[(h2+4)*9+(h1+4)]->count[OC_switch]+=sgn;//LEAK//LEAK
			//normalization factor increasing, necessary to be here
			for (int m=(h1+4);m<81;m+=9)//LEAK
				featurevector->markov[m]->factor[OC_switch]+=sgn;//LEAK//LEAK
		}
		//vertical markov features
		if (m_vertical){
			h1=abs(DCTs_row[rowindex])-abs(DCTs_row[rowindex+ImgSize.width]);
			h2=abs(DCTs_row[rowindex+ImgSize.width])-abs(DCTs_row[rowindex+2*ImgSize.width]);
			if (h1<-4)h1=-4;//LEAK
			else if (h1>4) h1=4;//LEAK
			if (h2<-4) h2=-4;//LEAK
			else if (h2>4) h2=4;//LEAK
			featurevector->markov[81+(h2+4)*9+(h1+4)]->count[OC_switch]+=sgn;//LEAK//LEAK
			//normalization factor increasing, necessary to be here
			for (int m=81+(h1+4);m<162;m+=9)//LEAK
				featurevector->markov[m]->factor[OC_switch]+=sgn;//LEAK//LEAK
		}
		//diagonal markov features
		if (m_diagonal){
			h1=abs(DCTs_row[rowindex])-abs(DCTs_row[rowindex+ImgSize.width+1]);
			h2=abs(DCTs_row[rowindex+ImgSize.width+1])-abs(DCTs_row[rowindex+2*ImgSize.width+2]);
			if (h1<-4)h1=-4;//LEAK
			else if (h1>4) h1=4;//LEAK
			if (h2<-4) h2=-4;//LEAK
			else if (h2>4) h2=4;//LEAK
			featurevector->markov[162+(h2+4)*9+(h1+4)]->count[OC_switch]+=sgn;//LEAK//LEAK
			//normalization factor increasing, necessary to be here
			for (int m=162+(h1+4);m<243;m+=9)//LEAK
				featurevector->markov[m]->factor[OC_switch]+=sgn;//LEAK//LEAK
		}
		//semidiagonal markov features
		if (m_semidiagonal){
			h1=abs(DCTs_row[rowindex])-abs(DCTs_row[rowindex-ImgSize.width+1]);
			h2=abs(DCTs_row[rowindex-ImgSize.width+1])-abs(DCTs_row[rowindex-2*ImgSize.width+2]);
			if (h1<-4)h1=-4;//LEAK
			else if (h1>4) h1=4;//LEAK
			if (h2<-4) h2=-4;//LEAK
			else if (h2>4) h2=4;//LEAK
			featurevector->markov[243+(h2+4)*9+(h1+4)]->count[OC_switch]+=sgn;//LEAK//LEAK
			//normalization factor increasing, necessary to be here
			for (int m=243+(h1+4);m<324;m+=9)//LEAK
				featurevector->markov[m]->factor[OC_switch]+=sgn;//LEAK//LEAK
		}

    //////// END OF THE NEW MARKOV FEATURES IMPLEMENTATION
    //////// DATE: 05-02-2007
    /////////////////////////////////////////////////////////////////

    }//if (what_features!="spatial_only")
}

/** Get the quantization table. 
 * @return the pointer to the quantization table (Ipp16u*)
 */
Ipp16u* cimage::getQuantizationTable(){
  return QuantizationTable;
}

/**
 * batch 274 feature extraction
 *
 * @param directory Directory containing JPEG images
 * @param file_out Path to the file the extracted features should be appended to
 * @param compressor What DCT implementation should be used - "ipp","matlab"
 * @param featureset What features should be extracted - "274" or "318"-additional local histograms
 * @param precision Precision of the features
 * @param crophow_v What vertical cropping should be used?
 * @param crophow_h What horizontal cropping should be used?
 * @param calitype Calibration type (1 or 2)
 */
int BatchFeatureExtraction(string directory, string file_out, char* compressor, string featureset, int precision, int crophow_v, int crophow_h, int calitype){
    bfs::directory_iterator end_iter;
    for ( bfs::directory_iterator dir_itr(directory); dir_itr != end_iter; ++dir_itr )
    {
      try
      {
        if ( bfs::is_directory( *dir_itr ) )
        {
          //++dir_count;
        }
        else
        {
			string pathh = directory + "/" + dir_itr->leaf();
			int f = int(pathh.find("\\"));
			while (f>-1){
				pathh.replace(f,1,"/");
				f = int(pathh.find("\\"));
			}
			 //is it ".jpg" file?
			if ((pathh.compare(pathh.length()-4,4,".jpg",0,4)==0) || (pathh.compare(pathh.length()-4,4,".JPG",0,4)==0)){
				//++file_count;
				cout << "processing " << dir_itr->leaf() << "\n";
				cimage image(pathh);
                                //Ipp16u* ZeroQuantTable = 0;
				    //cout << "Number of DCT blocks: " << image.NumberOfDCTBlocks.width << " x " << image.NumberOfDCTBlocks.height << endl;
				    //cout << "Image size: " << image.ImgSize.width << " x " << image.ImgSize.height << endl;
                                //image.Pevnak();
				    //cout << "Calibration type " << calitype << endl;
			    
                                image.ComputeFeatures(compressor,"merged",image.getQuantizationTable(),0,crophow_v,crophow_h);

				//image.ComputeFeatures(compressor);
				image.ExportFeatures(file_out,featureset,precision,calitype);
				image.Release();
			}
        }
      }
      catch ( const exception & ex )
      {
        //++err_count;
        cout << dir_itr->leaf() << " " << ex.what() << endl;
      }
    }
  return 0;
}

int cimage::Pevnak(){
	/*convert features to the normal notation */
//	Block2Row(DCTs,DCTs_row,NumberOfDCTBlocks);
	DCT2Spatial(DCTs,Pixels,NumberOfDCTBlocks,QuantizationTable,"matlab");

//	DCT2Spatial(Ipp16s* source,Ipp8u* target,IppiSize size, Ipp16u* QTable,char* compressor)

	for (int i=0;i<ImgSize.height;i++){
		for (int j=0;j<ImgSize.width;j++){
			cout <<(int)Pixels[(i/8)*NumberOfDCTBlocks.width*64+(j/8)*64+(i%8)*8+(j%8)]<<" ";
			//cout << (int) DCTs_row[i*NumberOfDCTBlocks.width+j] << " ";
		}
		cout <<endl;
	}
return 0;
	
}

/**
 * 274 feature extraction from single JPEG image only.
 *
 * @param input Single input JPEG image
 * @param file_out Path to the file the extracted features should be appended to
 * @param compressor What DCT implementation should be used - "ipp","matlab"
 * @param featureset What features should be extracted - "274" or "318"-additional local histograms
 * @param precision Precision of the features
 * @param crophow_v What vertical cropping should be used?
 * @param crophow_h What horizontal cropping should be used?
 * @param calitype Calibration type (1 or 2)
 */
int SingleFeatureExtraction(string input, string file_out, char* compressor, string featureset, int precision, int crophow_v, int crophow_h, int calitype){
    try
    {
        int f = int(input.find("\\"));
        while (f>-1){
          input.replace(f,1,"/");
          f = int(input.find("\\"));
        }
        cout << "processing\n";
        cimage image(input);
        image.ComputeFeatures(compressor,"merged",image.getQuantizationTable(),0,crophow_v,crophow_h);
        image.ExportFeatures(file_out,featureset,precision,calitype);
        image.Release();
    }
    catch ( const exception & ex )
    {
      throw TException(ex.what());
    }
  return 0;
}
