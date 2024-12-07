#include "term_document.hpp"
#include <cmath>

TermDocument::TermDocument(size_t docId) : m_doc_id(docId), m_tf(0ul) {}

TermDocument::~TermDocument()
{
}

size_t TermDocument::getDocId() const noexcept
{
    return m_doc_id;
}

void TermDocument::increaseFrequency()
{
    ++m_tf;
}

void TermDocument::computeTfIdf(double idf) noexcept
{
    auto term_weight = (1 + log10(m_tf));
    m_tf_idf = term_weight * idf;
}

double TermDocument::getTfIdf() const noexcept
{
    return m_tf_idf;
}
