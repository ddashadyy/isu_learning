#include "term_document.hpp"
#include <cmath>

TermDocument::TermDocument(size_t docId) : m_doc_id(docId), m_tf(0ul), m_tf_idf(1.0) {}

TermDocument::~TermDocument()
{
}

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

std::ostream& operator << (std::ostream& os, const TermDocument& td)
{
    return os << td.getDocId() << " " << td.getTfIdf();
}