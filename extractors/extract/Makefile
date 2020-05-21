SOURCEDIR=src
CURRENTDIR=$(PWD)

all:
	cd $(SOURCEDIR); make; cp jet $(CURRENTDIR)/extract; cd $(CURRENTDIR)

clean:
	cd $(SOURCEDIR); make clean; cd $(CURRENTDIR); rm -f jet *~ extract
