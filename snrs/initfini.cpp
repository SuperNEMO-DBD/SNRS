// Bayeux:
#include <bayeux/bayeux.h>
#include <bayeux/datatools/library_info.h>
#include <bayeux/datatools/kernel.h>

// This project:
#include <snrs/config.hpp>

class InitFini
{
public:
  
  InitFini()
  {
    bayeux::initialize();
    datatools::kernel & k = datatools::kernel::instance();
    datatools::library_info & kLibInfo = k.grab_library_info_register();
    auto libInfoData = kLibInfo.registration("snrs",
                                             "The SNRS library",
                                             SNRS_VERSION,
                                             "",
                                             "",
                                             SNRS_RESOURCE_PATH);
    libInfoData.store(datatools::library_info::keys::env_resource_dir(),
                      "SNRS_RESOURCE_PATH");
    libInfoData.print_tree(std::cerr);
  }
  
  ~InitFini()
  {
    bayeux::terminate();
  }
  
};

InitFini initFini;
