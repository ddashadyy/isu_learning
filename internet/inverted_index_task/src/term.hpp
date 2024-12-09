#pragma once

#include "term_document.hpp"
#include <list>
#include <memory>

class Term
{
public:
    Term( size_t docId );
    ~Term();

    void addDocument( size_t docId ) noexcept;
    void computeTfIdf(double idf) noexcept;

    size_t getDocumentFrequency() noexcept;
    std::list<TermDocument>& getList() noexcept;

private:
    std::list<TermDocument> m_list;
};
