#include "document_relevance.hpp"

DocumentRelevance::DocumentRelevance(size_t docId) : m_doc_id(docId), m_relevance(0.0) {}

DocumentRelevance::~DocumentRelevance() {}

size_t DocumentRelevance::getDocId() const noexcept
{
    return m_doc_id;
}

double DocumentRelevance::getRelevance() const noexcept
{
    return m_relevance;
}

void DocumentRelevance::updateRelevance(double tf) noexcept
{
    m_relevance += tf;
}

std::ostream& operator << (std::ostream& os, const DocumentRelevance& dr)
{
    return os << dr.getDocId() << " - " << dr.getRelevance() << ";";
}