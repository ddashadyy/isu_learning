#include "term.hpp"
#include <algorithm>

Term::Term(size_t docId) : m_term_frequency_in_collection(1ul)
{
    
    auto term_document_ptr = std::make_shared<TermDocument>(docId);
    m_list.push_back(term_document_ptr);
}

Term::~Term() {}

void Term::addDocument(size_t docId) noexcept
{
    auto& last_element = m_list.back();
    if (last_element->getDocId() == docId) last_element->increaseFrequency();
    else m_list.push_back(std::make_shared<TermDocument>(docId));

    ++m_term_frequency_in_collection;
}

void Term::computeTfIdf(double idf) noexcept
{
    std::for_each(m_list.begin(), m_list.end(), [&](std::shared_ptr<TermDocument>& td_ptr){
        td_ptr->computeTfIdf(idf);
    });
}

size_t Term::getDocumentFrequency() noexcept
{
    return m_term_frequency_in_collection;
}

std::list<std::shared_ptr<TermDocument>>& Term::getList() noexcept
{
    return m_list;
}
