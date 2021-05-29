// Bayeux:
#include <bayeux/bayeux.h>
#include <bayeux/datatools/library_info.h>
#include <bayeux/datatools/kernel.h>

// This project:
#include <snrs/config.hpp>

namespace snrs {
 
  class InitFini
  {
  public:
  
    InitFini()
    {
      if (not bayeux::is_initialized()) {
        bayeux::initialize();
        _bxinit_ = true;
      }
      datatools::kernel & k = datatools::kernel::instance();
      datatools::library_info & kLibInfo = k.grab_library_info_register();
      datatools::properties & libInfoData = kLibInfo.registration("snrs",
                                                                  "The SNRS library",
                                                                  SNRS_VERSION,
                                                                  "",
                                                                  "",
                                                                  SNRS_RESOURCE_PATH);
      libInfoData.store(datatools::library_info::keys::env_resource_dir(),
                        "SNRS_RESOURCE_PATH");
      libInfoData.store("truc", "foo");
      // std::cerr << "Library info :\n";
      // libInfoData.print_tree(std::cerr);
    }
  
    ~InitFini()
    {
      if (_bxinit_) {
        bayeux::terminate();
      }
    }

    bool _bxinit_ = false;
  
  };

} // namespace snrs

namespace { 
  snrs::InitFini _snrsInitFini;
}
