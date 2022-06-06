#include "serializer.h"
namespace lyon {

Serializer::Serializer(size_t ba_size, bool fix) : m_fix(fix) {
    m_ba = std::make_shared<ByteArray>(ba_size);
}

void Serializer::setPosition(size_t position) { m_ba->setPosition(position); }

void Serializer::clear() { m_ba->clear(); }

void Serializer::loadFromStr(const std::string &content) {
    m_ba->loadFromStr(content);
}

} // namespace lyon
