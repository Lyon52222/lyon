#include "serializer.h"
namespace lyon {

Serializer::Serializer(size_t ba_size, bool fix) : m_fix(fix) {
    m_ba = std::make_shared<ByteArray>();
}

Serializer::Serializer(ByteArray::ptr ba, bool fix) : m_fix(fix) { m_ba = ba; }

Serializer::Serializer(const std::string &str, bool fix) : m_fix(fix) {
    m_ba = std::make_shared<ByteArray>();
    m_ba->loadFromStr(str);
}
void Serializer::setPosition(size_t position) { m_ba->setPosition(position); }

void Serializer::clear() { m_ba->clear(); }

void Serializer::loadFromStr(const std::string &content) {
    m_ba->loadFromStr(content);
}
std::string Serializer::toString() { return std::move(m_ba->toString(0)); }

} // namespace lyon
