/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2008
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#ifndef CIMAGE_H
#define CIMAGE_H

extern "C" {
#include "jpeglib.h"  //application interface for the JPEG library
}
#include "jmemsys.h"
#include "cfeaturevector.h"
class cimage{
  public:
    int Pevnak();
    std::string imagename; ///< Name of the image file
    IppiSize NumberOfDCTBlocks; ///< Number of DCT blocks
    IppiSize ImgSize; ///< Size of the image in pixels (width and height)
  /** Construct image object - load the image from the given file.
    @param filename_in File (path) of the JPEG image to be loaded.
   */
    cimage(std::string filename_in);
    /** Destructor of the class cimage. */
    ~cimage();
    /** Release the image without saving. */
    void Release();
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
void ComputeFeatures(char* compressor, char *FeatureSet, Ipp16u *QuantTable1, Ipp16u *QuantTable2, int crophow_v, int crophow_h);
/**
 * Export features into the given file.
 *
 * @param filename Export file path
 * @param featureset What featureset should be exported ("274" or "318"-additional local histograms)
 * @param precision Precision of the features
 * @param calitype Calibration type (1=merged,2=cc-merged)
 */
void ExportFeatures(std::string filename, std::string featureset, int precision, int calitype);
  /** Get the quantization table. 
    * @return the pointer to the quantization table (Ipp16u*)
    */
  Ipp16u* getQuantizationTable();
  
private:
        struct jpeg_decompress_struct srcinfo;
	struct jpeg_compress_struct dstinfo;
	struct jpeg_error_mgr jsrcerr, jdsterr;
	jvirt_barray_ptr * src_coef_arrays;
	jvirt_barray_ptr * dst_coef_arrays;
	FILE * input_file;
// 	FILE * output_file;
	Ipp16u* QuantizationTable; ///< Pointer to the quantization table
	Ipp16s* DCTs;              ///< Pointer to the field of DCT coefficients (block by block structure)
	Ipp16s* DCTs_row;          ///< Pointer to the field of DCT coefficients (row by row structure)
	Ipp8u* Pixels;             ///< Pointer to the field of pixel values - spatial domain (block by block structure)
        Ipp8u* Pixels_row;         ///< Pointer to the field of pixel values - spatial domain (row by row structure)
	Ipp16s* CroppedDCTs;       ///< Pointer to the field of DCT coefficients of the cropped image (block by block structure)
	Ipp16s* CroppedDCTs_row;   ///< Pointer to the field of DCT coefficients of the cropped image (row by row structure)
	Ipp8u* CroppedPixels;      ///< Pointer to the field of pixel values - spatial domain - of the cropped image (block by block structure)
	Ipp8u* CroppedPixels_row;  ///< Pointer to the field of pixel values - spatial domain - of the cropped image (row by row structure)
	IppiSize NumberOfCroppedDCTBlocks; ///< Number of DCT blocks in the cropped version of the image
 	IppiSize CroppedImgSize;  ///< Size of the cropped image in pixels (width and height)
	cfeaturevector* featurevector; ///< Feature vector structure
	int NumberOfColorComponents; ///< Number of color components of the image
     bool AlreadyReleased; ///< True when the current image was already released => there will be no Release() in destructor any more
   bool MsgTooLong; ///< Indicator that the message to being embedded is too long for the image
/**
 * Decompress DCT blocks into the spatial domain using source and target.
 *
 * @param source Pointer to the source field of DCT coefficients (8x8 block structure)
 * @param target Pointer to the target field of pixels (8x8 block structure)
 * @param size Number of DCT blocks in the source field (width and height of the field)
 * @param QTable Quantization table being considered
 * @param compressor What DCT implementation should be used
 */
void DCT2Spatial(Ipp16s* source,Ipp8u* target,IppiSize size, Ipp16u* QTable,char* compressor);
/**
 * Compress spatial image into DCT blocks using source and target.
 *
 * @param source Pointer to the source field of pixel values (8x8 block structure)
 * @param target Pointer to the target field of DCT coefficients (8x8 block structure)
 * @param size Number of DCT blocks in the target field (width and height of the field)
 * @param QTable Quantization table being considered
 * @param compressor What DCT implementation should be used
 */
void Spatial2DCT(Ipp8u*source,Ipp16s* target, IppiSize size, Ipp16u* QTable,char* compressor);
/**
 * Forward DCT transform of 8x8 block, including -128 level shift and quantization.
 *
 * @param source Pointer to the source 8x8 block
 * @param target Pointer to the target 8x8 block
 * @param QTable Quantization table
 * @param compressor What DCT transform implementation should be used - "ipp","matlab"
 */
void DCT_8x8(Ipp8u* source,Ipp16s* target, Ipp16u* QTable,char* compressor);
/**
 * Inverse DCT transform of 8x8 block, including +128 level shift and dequantization.
 *
 * @param source Pointer to the source 8x8 block
 * @param target Pointer to the target 8x8 block
 * @param QTable Quantization table
 * @param compressor What DCT transform implementation should be used - "ipp","matlab"
 */
void iDCT_8x8(Ipp16s* source,Ipp8u* target, Ipp16u* QTable,char* compressor);
/**
 * Transform the image from 8x8 block format to the 'row by row' format - Ipp8u variant.
 *
 * @param source Pointer to the source image in 8x8 block format
 * @param target Pointer to the target image
 * @param size Number of 8x8 blocks in the source field (width and height of the field)
 */
void Block2Row(Ipp8u* source, Ipp8u* target, IppiSize size);
/**
 * Transform the image from 8x8 block format to the 'row by row' format - Ipp16s variant.
 *
 * @param source Pointer to the source image in 8x8 block format
 * @param target Pointer to the target image
 * @param size Number of 8x8 blocks in the source field (width and height of the field)
 */
void Block2Row(Ipp16s* source, Ipp16s* target, IppiSize size);
/**
 * Transform the image from the 'row by row' to the 8x8 block format - Ipp8u variant.
 *
 * @param source Pointer to the source image in the 'row by row'
 * @param target Pointer to the target image
 * @param size Number of 8x8 blocks in the target field (width and height of the field)
 */
void Row2Block(Ipp8u* source, Ipp8u* target, IppiSize size);
/**
 * Transform the image from the 'row by row' to the 8x8 block format - Ipp16s variant.
 *
 * @param source Pointer to the source image in the 'row by row'
 * @param target Pointer to the target image
 * @param size Number of 8x8 blocks in the target field (width and height of the field)
 */
void Row2Block(Ipp16s* source, Ipp16s* target, IppiSize size);
/**
 * Crop the image.
 *
 * @param source Pointer to the source image (row by row format)
 * @param target Pointer to the image being created by cropping (row by row format)
 * @param size Number of 8x8 blocks in the original image (width and height of the field)
 * @param crophow_v What vertical cropping should be used?
 * @param crophow_h What horizontal cropping should be used?
 */
void Crop(Ipp8u* source,Ipp8u* target,IppiSize size,int crophow_v, int crophow_h);/* crop the image */
/**
 * Converts block coordinates [i,j,k] into the corresponding index in the row representation.
 *
 * @param i 1st block coordinate
 * @param j 2nd block coordinate
 * @param k 3rd block coordinate
 * @param size Size of the block structure
 * @return the index (in the row representation) corresponding to the block coordinates [i,j,k]
 */
Ipp32s IndexBlock2Row(int i, int j, int k, IppiSize size);
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
void LocalCorrectionOfFeatureVector(int OC_switch, int i, int j, int k, IppiSize NumberOfBlocks, IppiSize ImgSize, Ipp16s* DCTs, Ipp16s* DCTs_row, Ipp8u* Pixels, char* operation, char* what_features);
};

/**
 * Batch 274 feature extraction from all the images in the given directory.
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
int BatchFeatureExtraction(std::string directory, std::string file_out, char* compressor, std::string featureset, int precision, int crophow_v, int crophow_h, int calitype);

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
int SingleFeatureExtraction(std::string input, std::string file_out, char* compressor, std::string featureset, int precision, int crophow_v, int crophow_h, int calitype);

struct jvirt_barray_control {
  JBLOCKARRAY mem_buffer;	/* => the in-memory buffer */
  JDIMENSION rows_in_array;	/* total virtual array height */
  JDIMENSION blocksperrow;	/* width of array (and of memory buffer) */
  JDIMENSION maxaccess;		/* max rows accessed by access_virt_barray */
  JDIMENSION rows_in_mem;	/* height of memory buffer */
  JDIMENSION rowsperchunk;	/* allocation chunk size in mem_buffer */
  JDIMENSION cur_start_row;	/* first logical row # in the buffer */
  JDIMENSION first_undef_row;	/* row # of first uninitialized row */
  boolean pre_zero;		/* pre-zero mode requested? */
  boolean dirty;		/* do current buffer contents need written? */
  boolean b_s_open;		/* is backing-store data valid? */
  jvirt_barray_ptr next;	/* link to next virtual barray control block */
  backing_store_info b_s_info;	/* System-dependent control info */
};


#endif // CIMAGE_H
