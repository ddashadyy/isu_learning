#pragma once

#include "term_document.hpp"
#include <list>
#include <memory>

class Term final
{
public:
    Term() = default;
    Term( size_t docId );
    ~Term();

    void addDocument( size_t docId ) noexcept;
    void computeTfIdf(double idf) noexcept;

    size_t getDocumentFrequency() noexcept;
    std::list<TermDocument>& getList() noexcept;

    json toJson() const;
    static Term fromJson( const json& jsonFile );

private:
    std::list<TermDocument> m_list;
};
