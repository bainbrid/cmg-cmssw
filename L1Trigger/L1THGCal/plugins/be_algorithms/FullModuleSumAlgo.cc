#include "L1Trigger/L1THGCal/interface/HGCalTriggerBackendAlgorithmBase.h"
#include "L1Trigger/L1THGCal/interface/fe_codecs/HGCalBestChoiceCodec.h"
#include "DataFormats/ForwardDetId/interface/HGCTriggerHexDetId.h"

#include "DataFormats/L1THGCal/interface/HGCalCluster.h"

using namespace HGCalTriggerBackend;

class FullModuleSumAlgo : public Algorithm<HGCalBestChoiceCodec> 
{
    public:

        FullModuleSumAlgo(const edm::ParameterSet& conf, const HGCalTriggerGeometryBase* const geom):
            Algorithm<HGCalBestChoiceCodec>(conf,geom),
            cluster_product_( new l1t::HGCalClusterBxCollection ){}

        virtual void setProduces(edm::EDProducer& prod) const override final 
        {
            prod.produces<l1t::HGCalClusterBxCollection>(name());
        }

        virtual void run(const l1t::HGCFETriggerDigiCollection& coll) override final;

        virtual void putInEvent(edm::Event& evt) override final 
        {
            evt.put(std::move(cluster_product_),name());
        }

        virtual void reset() override final 
        {
            cluster_product_.reset( new l1t::HGCalClusterBxCollection );
        }

    private:
        std::unique_ptr<l1t::HGCalClusterBxCollection> cluster_product_;

};

/*****************************************************************/
void FullModuleSumAlgo::run(const l1t::HGCFETriggerDigiCollection& coll) 
/*****************************************************************/
{
    for( const auto& digi : coll ) 
    {
        HGCalBestChoiceCodec::data_type data;
        data.reset();
        const HGCTriggerHexDetId& moduleId = digi.getDetId<HGCTriggerHexDetId>();
        digi.decode(codec_, data);

        // Sum of trigger cells inside the module
        uint32_t moduleSum = 0;
        for(const auto& value : data.payload)
        {
            moduleSum += value;
        }
        // dummy cluster without position
        // moduleId filled in place of hardware eta
        l1t::HGCalCluster cluster( reco::LeafCandidate::LorentzVector(), 
                moduleSum, moduleId, 0);

        cluster_product_->push_back(0,cluster);
    }
}

DEFINE_EDM_PLUGIN(HGCalTriggerBackendAlgorithmFactory, 
        FullModuleSumAlgo,
        "FullModuleSumAlgo");
