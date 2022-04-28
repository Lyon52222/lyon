#ifndef __LYON_CONFIG_H__
#define __LYON_CONFIG_H__

#include <memory>
namespace lyon {
class ConfigVarBase {
  public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
};

} // namespace lyon
#endif // !__LYON_CONFIG_H__
