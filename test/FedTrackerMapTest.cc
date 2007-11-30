#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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

#include "CondFormats/SiStripObjects/interface/SiStripApvGain.h"
#include "CondFormats/DataRecord/interface/SiStripApvGainRcd.h"

using namespace std;
using namespace edm;

class FedTrackerMapTest : public edm::EDAnalyzer {
public:
  explicit FedTrackerMapTest( const edm::ParameterSet& );
  ~FedTrackerMapTest();


  virtual void analyze( const edm::Event&, const edm::EventSetup& );
private:
  // ----------member data ---------------------------
  edm::ParameterSet pset;

};


//
FedTrackerMapTest::FedTrackerMapTest( const edm::ParameterSet& iConfig )
{
  //now do what ever initialization is needed
  pset = iConfig.getParameter<edm::ParameterSet>("TkmapParameters");


}
FedTrackerMapTest::~FedTrackerMapTest()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


// ------------ method called to produce the data  ------------

void
FedTrackerMapTest::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{

  edm::ESHandle<SiStripFedCabling> fedcabling;
  iSetup.get<SiStripFedCablingRcd>().get(fedcabling );
  edm::LogInfo("FedTrackerMapTest") << "[FedTrackerMapTest::analyze] End Reading SiStripFedCabling" << std::endl;

  edm::ESHandle<SiStripApvGain> SiStripApvGain_;
  iSetup.get<SiStripApvGainRcd>().get(SiStripApvGain_);
  edm::LogInfo("FedTrackerMapTest") << "[FedTrackerMapTest::analyze] End Reading SiStripApvGain" << std::endl;

  TrackerMap * tkMap = new TrackerMap(pset,fedcabling);


  const vector<uint16_t>& feds = fedcabling->feds();
  vector<uint16_t>::const_iterator ifed;
  for ( ifed = feds.begin(); ifed != feds.end(); ifed++ ) { // iterate over active feds, get all their FedChannelConnection-s
    const vector<FedChannelConnection>& conns = fedcabling->connections( *ifed );
    vector<FedChannelConnection>::const_iterator iconn;
    for ( iconn = conns.begin(); iconn != conns.end(); iconn++ ) { // loop over FedChannelConnection objects
	
      if( iconn->detId()==0 ||  
	  iconn->detId() == sistrip::invalid32_ ||  
	  iconn->apvPairNumber() == sistrip::invalid_  ||
	  iconn->nApvPairs() == sistrip::invalid_ ) {
	continue;
      }


      SiStripApvGain::Range range=SiStripApvGain_->getRange( iconn->detId());

      //      std::cout << "filling fedid " << iconn->fedId() << " fedch " << iconn->fedCh() << " value " <<  SiStripApvGain_->getApvGain(iconn->apvPairNumber()*2,range) << std::endl;
      
      tkMap->fill_fed_channel(iconn->fedId(),
			      iconn->fedCh(),
			      SiStripApvGain_->getApvGain(iconn->apvPairNumber()*2,range));
      
    }
  }

  tkMap->save_as_fedtrackermap(true,0.,0.,"fedsvgmap2.png",3000,1600);
  //tkMap->save_as_fedtrackermap(true,0.,0.,"fedsvgmap2.pdf",3000,1600);

}

//define this as a plug-in
DEFINE_FWK_MODULE(FedTrackerMapTest);

