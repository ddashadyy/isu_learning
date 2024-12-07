#pragma once

#include <cstddef>

// private int docID – идентификатор документа;
// private double relevance – степень соответствия документа запросу;
// public DocumentRelevance(int docID) – конструктор с параметром задает идентификатор документа;
// public int getDocID() – возвращает идентификатор документа;
// public double getRelevance() – возвращает меру соответствия документа запросу;
// public void updateRelevance(double tf) – увеличивает релевантность текущего документа на значение tf;
// public String toString() – метод преобразования объекта к строке, возвращает идентификатор документа и релевантность.


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

// вернуться к оператору вывода 