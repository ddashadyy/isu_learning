#pragma once

#include <cstddef>
#include <string>
#include <ostream>

class TermDocument
{
public:
    TermDocument( size_t docId );
    ~TermDocument();
    
    size_t getDocId() const noexcept;
    void increaseFrequency() noexcept;
    void computeTfIdf( double idf ) noexcept;
    double getTfIdf() const noexcept;

private:
    size_t m_doc_id;
    size_t m_tf;
    double m_tf_idf;
};

std::ostream& operator << (std::ostream& os, const TermDocument& td);


