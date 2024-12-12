#include "term_document.hpp"
#include <cmath>

TermDocument::TermDocument(size_t docId) : m_doc_id(docId), m_tf(1ul), m_tf_idf(1.0) {}

TermDocument::~TermDocument() {}

size_t TermDocument::getDocId() const noexcept
{
    return m_doc_id;
}

void TermDocument::increaseFrequency() noexcept
{
    ++m_tf;
}

void TermDocument::computeTfIdf(double idf) noexcept
{
    auto term_weight = m_tf > 0 ? (1 + log10(m_tf)) : 0;
    m_tf_idf = term_weight * idf;
}

double TermDocument::getTfIdf() const noexcept
{
    return m_tf_idf;
}

json TermDocument::toJson() const
{
    return json{{"doc_id", m_doc_id}, {"tf", m_tf}, {"tf_idf", m_tf_idf}};
}

TermDocument TermDocument::fromJson(const json& jsonFile)
{
    TermDocument termDoc(jsonFile.at("doc_id").get<size_t>());
    termDoc.m_tf = jsonFile.at("tf").get<size_t>();
    termDoc.m_tf_idf = jsonFile.at("tf_idf").get<double>();

    return termDoc;
}

std::ostream& operator << (std::ostream& os, const TermDocument& td)
{
    return os << td.getDocId() << " " << td.getTfIdf();
}
