#pragma once

#include <cstddef>
#include <ostream>

class DocumentRelevance
{
public:
    DocumentRelevance( size_t docId );
    ~DocumentRelevance();

    size_t getDocId() const noexcept;
    double getRelevance() const noexcept;
    void updateRelevance( double tf ) noexcept;

private:
    size_t m_doc_id;
    double m_relevance;
};

std::ostream& operator << (std::ostream& os, const DocumentRelevance& dr);

// вернуться к оператору вывода 