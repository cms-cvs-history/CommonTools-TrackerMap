#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "DataFormats/GeometrySurface/interface/BoundSurface.h"
#include "CommonTools/TrackerMap/interface/TrackerMap.h"
#include "CommonTools/TrackerMap/interface/TmModule.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CondFormats/DataRecord/interface/SiStripFedCablingRcd.h"
#include "CondFormats/SiStripObjects/interface/FedChannelConnection.h"
#include "CommonTools/TrackerMap/interface/TmApvPair.h"
class TrackerGeometryTest : public edm::EDAnalyzer {
   public:
      explicit TrackerGeometryTest( const edm::ParameterSet& );
      ~TrackerGeometryTest();


      virtual void analyze( const edm::Event&, const edm::EventSetup& );
   private:
      // ----------member data ---------------------------
 edm::ParameterSet pset;

};


//
TrackerGeometryTest::TrackerGeometryTest( const edm::ParameterSet& iConfig )
{
   //now do what ever initialization is needed
pset = iConfig.getParameter<edm::ParameterSet>("TkmapParameters");


}
TrackerGeometryTest::~TrackerGeometryTest()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// ------------ method called to produce the data  ------------

void
TrackerGeometryTest::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
 edm::ESHandle<SiStripFedCabling> pDD1;
 iSetup.get<SiStripFedCablingRcd>().get(pDD1 );
 TrackerMap * tkMap = new TrackerMap();//trackermap without fed processing
  TrackerMap * tkMap1 = new TrackerMap(pset,pDD1);
  TrackerMap * tkMap2 = new TrackerMap(pset,pDD1);

//  TrackerMap * tkMap = new TrackerMap( "GeomDetUnitId : move the mouse on the module and look at the second text line below");
//  TrackerMap * tkMap1 = new TrackerMap( "Default rainbow palette test");
  tkMap1->setPalette(1);
   using namespace edm;

   edm::LogInfo("TrackerGeometryTest")<< "Here I am";

   //
   // get the TrackerGeom
   //
   edm::ESHandle<TrackerGeometry> pDD;
   iSetup.get<TrackerDigiGeometryRecord>().get( pDD );
   edm::LogInfo("TrackerGeometryTest")<< " Geometry node for TrackerGeom is  "<<&(*pDD);
   edm::LogInfo("TrackerGeometryTest")<<" I have "<<pDD->dets().size() <<" detectors";
   edm::LogInfo("TrackerGeometryTest")<<" I have "<<pDD->detTypes().size() <<" types";
  float width,length,thickness,widthAtHalfLength;
  int spicchif[] ={24,24,40,56,40,56,80};
  int spicchib[] ={20,32,44,30,38,46,56,42,48,54,60,66,74};

  int nlay=0;
  int layer,subdet,leftright=0,ringno,petalno,moduleno,isStereo,pixel_strip,barrel_forward;
  string name0,name1,name2,name3,name4,name5;
  int ring = 0;
  int forback;
  int nmod = 0;
  int idmod = 0;
  int ntotmod = 0;
  float r;
  int bar_fow = 1;
  float phi,phi1;
  float rmedioS[]={0.27665, 0.3671, 0.4474, 0.5617, 0.6768, 0.8189, 0.9907};
  float rmedioP[]={0.0623081, 0.074111,  0.0870344, 0.103416, 0.115766, 0.132728, 0.140506};
   string nameDet;
  int last_layer=0;
   std::cout << "[TrackerGeometryTest] Start dumping geometry " << std::endl;
   //
   // get the CmsDigiTracker
   //
   //
   std::vector<GeomDetUnit*>::const_iterator begin = pDD->detUnits().begin();
   std::vector<GeomDetUnit*>::const_iterator end = pDD->detUnits().end();
   for ( ; begin != end; ++begin) {
     ntotmod++;
	    
          subdet = (*begin)->geographicalId().subdetId();
          idmod = (*begin)->geographicalId().rawId();
           if(subdet==1||subdet==3||subdet==5){//barrel 
         layer = ((*begin)->geographicalId().rawId()>>16)&0xF;
         leftright=0;
         name0=" ";
          }else{ //endcap
         leftright = ((*begin)->geographicalId().rawId()>>23)&0x3;
         name0="+z"; if (leftright==1) name0="-z"; 
         layer = ((*begin)->geographicalId().rawId()>>16)&0xF;
           }
         isStereo=(*begin)->geographicalId().rawId()&0x3;
         pixel_strip = 2;if(subdet<=2)pixel_strip=1;
         barrel_forward=2;if(subdet==2||subdet==4||subdet==6){if(leftright==1) barrel_forward=1; else barrel_forward=3;}
         nlay = TrackerMap::layerno(subdet,leftright,layer);
         ringno=0;
         petalno=0;
         moduleno=0;
         name1 = "   ";
if(subdet==1){nameDet="BPIX";name1=" ladder ";ringno=((*begin)->geographicalId().rawId()>>8)&0xFF;}
    if(subdet==2){nameDet="FPIX";name1=" ring ";ringno=((*begin)->geographicalId().rawId()>>8)&0xFF;}
    if(subdet==3){nameDet="TIB";name1=" string ";ringno=((*begin)->geographicalId().rawId()>>4)&0x3F;}
    if(subdet==5){nameDet="TOB";name1=" rod ";ringno=((*begin)->geographicalId().rawId()>>5)&0x7F;}
    if(subdet==4){nameDet="TID";name1=" ring ";ringno=((*begin)->geographicalId().rawId()>>9)&0x3;}
    if(subdet==6){nameDet="TEC";name1=" ring ";ringno=((*begin)->geographicalId().rawId()>>5)&0x7;}
         name2 = " ";
        if(subdet==6){name2=" petal "; petalno=((*begin)->geographicalId().rawId()>>8)&0xF;}
    if(subdet==2){name2=" blade "; petalno=((*begin)->geographicalId().rawId()>>10)&0x3F;}
         name3 = " ";
         if(subdet==6){name3=" forward ";if((((*begin)->geographicalId().rawId()>>4)&0x1)==1)name3=" backward "; }
         if(subdet==2){name3=" forward ";if((((*begin)->geographicalId().rawId()>>4)&0x1)==1)name3=" backward "; }
         name5 = " ";
          forback=0;
         if(subdet==6){name5=" forward ";if((((*begin)->geographicalId().rawId()>>12)&0x3)==1){name5=" backward ";forback=1; }}
         if(subdet==2){name5=" left ";if((((*begin)->geographicalId().rawId()>>14)&0x1)==1){name5=" right ";forback=1; }}
         if(subdet==3){name2=" neg ";if((((*begin)->geographicalId().rawId()>>15)&0x1)==1)name2=" pos "; }
         if(subdet==5){name2=" neg ";if((((*begin)->geographicalId().rawId()>>15)&0x1)==1)name2=" pos "; }
         if(subdet==3){name3=" internal ";if((((*begin)->geographicalId().rawId()>>14)&0x1)==1)name3=" external "; }
         if(subdet==4){name3=" forward ";if((((*begin)->geographicalId().rawId()>>7)&0x1)==1)name3=" backward "; }
if(subdet==1){moduleno=((*begin)->geographicalId().rawId() >>2)&0x3F; }
    if(subdet==3){moduleno=((*begin)->geographicalId().rawId() >>2)&0x3; }
    if(subdet==5){moduleno=((*begin)->geographicalId().rawId() >>2)&0x7; }
    if(subdet==4){moduleno=((*begin)->geographicalId().rawId() >>2)&0x1F; }
    if(subdet==6){moduleno=((*begin)->geographicalId().rawId() >>2)&0x7; }
    if(subdet==2){moduleno=((*begin)->geographicalId().rawId() >>2)&0x7; }
	 length = (*begin)->surface ().bounds ().length () / 100.0; // cm -> m
	 width = (*begin)->surface ().bounds ().width () / 100.0;   // cm -> mo
	 thickness = (*begin)->surface ().bounds ().thickness () / 100.0;  // cm -> m
         widthAtHalfLength=(*begin)->surface ().bounds ().widthAtHalfLength () / 100.0;
       // }
         if(nlay!=last_layer){ring = -1;last_layer=nlay;}
          bar_fow=2;
          if(nlay<16)bar_fow=1;
          if(nlay>15&&nlay<31)bar_fow=3;
      float posx =  (*begin)->surface().position().x()/ 100.0;  // cm -> m
      float posy =  (*begin)->surface().position().y()/ 100.0;  // cm -> m
       r=pow(((double)(posx*posx) + posy*posy),0.5);
      phi1=atan(posy/posx);
      phi=phi1;
      if(posy < 0.&&posx>0)phi=phi1+2.*M_PI;
      if(posx < 0.)phi=phi1+M_PI;
      if(fabs(posy)<0.000001&&posx>0)phi=0;
      if(fabs(posy)<0.000001&&posx<0)phi=M_PI;
      if(fabs(posx)<0.000001&&posy>0)phi=M_PI/2.;
      if(fabs(posx)<0.000001&&posy<0)phi=M_PI + M_PI/2.;

      if(bar_fow==2){ //barrel
      if(subdet==1)ring=moduleno;
      if(subdet==5){ring=moduleno;
         if((((*begin)->geographicalId().rawId()>>15)&0x1)==1)ring=ring+6;
         }
      if(subdet==3){
         ring=moduleno;
         if(layer==2||layer==4){
          if((((*begin)->geographicalId().rawId()>>14)&0x1)==1){ring=ring*2;} else {ring=ring*2-1;}
                        }else{
           if((((*begin)->geographicalId().rawId()>>14)&0x1)==1){ring=ring*2-1;} else {ring=ring*2;}
                     }

        // if( (((*begin)->geographicalId().rawId()>>14)&0x1)==1){ring=ring*2;} else {ring=ring*2-1;} ;
         if((((*begin)->geographicalId().rawId()>>15)&0x1)==1)ring=ring+6;
         }

         nmod=(int)((phi/(2.*M_PI))*spicchib[nlay-31]+.1)+1;
         if(subdet==1)nmod=ringno;
      } else { // endcap
      if (subdet==4||subdet==6) {
        for (int i=0;i<7; i++){
          if (fabs(r-rmedioS[i])<0.015){
            ring =i+1;
            break;
          }
        }
        nmod=(int)((phi/(2.*M_PI))*spicchif[ring-1]+.1)+1;
      }
      else{
        for (int i=0; i<7; i++){
          if(fabs(r -rmedioP[i])<0.0001){
            ring=i+1;
            break;
          }
        }
        nmod=(int)((phi/(2.*M_PI))*24+.1)+1;
      }
      } //end of endcap part
       if (isStereo==2) nmod=nmod+100;
        tkMap1->fill(idmod,float(nlay*nmod));
     name4=" "; if (isStereo==2)name4=" stereo";
         if(subdet==1)tkMap->fillc(idmod,0,255-ringno*5,ringno*5);
         if(subdet==3)tkMap->fillc(idmod,ringno*4,255-ringno*4,ringno*4);
         if(subdet==5)tkMap->fillc(idmod,(256-ringno*3),(256-ringno*3),(256-ringno*3));
         if(subdet==4)tkMap->fillc(idmod,256-moduleno*10,256-moduleno*10,0);
         if(subdet==2)tkMap->fillc(idmod,256-petalno*10,256-petalno*10,0);
         if(subdet==6){
          if(forback==0)tkMap->fillc(idmod,255,(160-petalno*20),(160-petalno*20));
          if(forback==1)tkMap->fillc(idmod,(160-petalno*20),255,255);
          } else {tkMap->fill(idmod,float(subdet));}
   }
const vector<unsigned short> feds = pDD1->feds();
  cout<<"SiStripFedCabling has "<< feds.size()<<" active FEDS"<<endl;
    int num_board=0;
    int num_crate;
  for(vector<unsigned short>::const_iterator ifed = feds.begin();ifed<feds.end();ifed++){
    const std::vector<FedChannelConnection> theconn = pDD1->connections( *ifed );
    for(std::vector<FedChannelConnection>::const_iterator iconn = theconn.begin();iconn<theconn.end();iconn++){
      if(iconn->apvPairNumber()==0)tkMap2->fill(tkMap2->module(iconn->fedId(),iconn->fedCh()),(float)num_board);
      int key = iconn->fedId()*1000+iconn->fedCh();
      TmApvPair* apvpair = tkMap2->apvMap[key];
      if(apvpair!=0)tkMap2->fill_fed_channel(iconn->fedId(),iconn->fedCh(),(float)apvpair->mod->layer);    
    }
    num_board++;
  }

    tkMap1->showPalette(true);
    tkMap1->printall(true,0.,0.,"tmap");
    tkMap2->showPalette(true);
    tkMap2->printall(true,0.,0.,"fakecabling");
    tkMap2->printonline();
//    tkMap2->print(true,0.,0.,"svgmap3");
    tkMap2->save_as_fedtrackermap(true,0.,0.,"fedsvgmap3.png",3000,1600);
    tkMap2->save_as_fedtrackermap(true,0.,0.,"fedsvgmap3.svg",3000,1600);
    tkMap2->save(true,0.,0.,"svgmap3.png",3000,1600);
//    tkMap->print(true,0.,0.,"svgmap1");
    tkMap->showPalette(true);
    tkMap->save(true,0.,0.,"svgmap1.svg");//default format svg without Javascript
    tkMap->save(true,0.,0.,"svgmap1.png",1500,800);
    tkMap->save(true,0.,0.,"svgmap1.jpg",1500,800);
    tkMap->save(true,0.,0.,"svgmap1.pdf",1500,800);
    tkMap1->showPalette(true);
 //   tkMap1->print(true,0.,0.,"svgmap2");
    tkMap1->save_as_fedtrackermap(true,0.,0.,"fedsvgmap2.png",3000,1600);
    tkMap1->save_as_fedtrackermap(true,0.,0.,"fedsvgmap2.pdf",3000,1600);
    tkMap1->save(true,0.,0.,"svgmap2.png",3000,1600);
    tkMap1->save(true,0.,0.,"svgmap2.pdf",3000,1600);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackerGeometryTest);
