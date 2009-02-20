# last update: $Date: 2008/10/29 17:28:42 $ by $Author: flucke $
import FWCore.ParameterSet.Config as cms

#_________________________________HLT bits___________________________________________
import HLTrigger.HLTfilters.hltHighLevel_cfi
ALCARECOTkAlCosmicsHLT = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone(
    andOr = True, ## choose logical OR between Triggerbits
    HLTPaths = ['HLT_TrackerCosmics'],
    throw = False # tolerate triggers stated above, but not available
    )
    
#________________________________Track selection____________________________________
# take from stream that is HLT-less
from Alignment.CommonAlignmentProducer.ALCARECOTkAlCosmics_cff import *

#________________________________Sequences____________________________________
# simply add HLT in front of HLT-less paths
seqALCARECOTkAlCosmicsCTFHLT = cms.Sequence(ALCARECOTkAlCosmicsHLT+seqALCARECOTkAlCosmicsCTF)
seqALCARECOTkAlCosmicsCosmicTFHLT = cms.Sequence(ALCARECOTkAlCosmicsHLT+seqALCARECOTkAlCosmicsCosmicTF)
seqALCARECOTkAlCosmicsRSHLT = cms.Sequence(ALCARECOTkAlCosmicsHLT+seqALCARECOTkAlCosmicsRS)

