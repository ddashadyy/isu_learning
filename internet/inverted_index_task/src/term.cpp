#include "term.hpp"
#include <algorithm>

Term::Term(size_t docId) 
{
    m_list.emplace_back(docId);
}

Term::~Term() {}

void Term::addDocument(size_t docId) noexcept
{
    if (!m_list.empty() && m_list.back().getDocId() == docId)
        m_list.back().increaseFrequency();
    else m_list.emplace_back(docId);

}

void Term::computeTfIdf(double idf) noexcept
{
    std::for_each(m_list.begin(), m_list.end(), [&](TermDocument& td) {
        td.computeTfIdf(idf);
    });
}

size_t Term::getDocumentFrequency() noexcept
{
    return m_list.size();
}


std::list<TermDocument>& Term::getList() noexcept
{
    return m_list;
}