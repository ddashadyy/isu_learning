import java.io.File;
import java.io.IOException;
import java.util.*;



public class InvertedIndex {

    private final List<String> documents = new LinkedList<>();
    public final Map<String, LinkedList<Integer>> index = new HashMap<>();

    public void indexDocument(String path) throws IOException {
        documents.add(path);
        File file = new File(path);
        Scanner fileScanner = null;

        try {
            fileScanner = new Scanner(file);

            String[] clearedFileContent = null;


            while (fileScanner.hasNext()) {
                clearedFileContent = fileScanner.next().split("[^0-9a-zA-Z]");
                for (String content : clearedFileContent) {
                    int DOCUMENT_INDEX = documents.size();
                    documents.add(path);
                    LinkedList<Integer> localList = index.computeIfAbsent(content, k -> new LinkedList<Integer>());
                    if(!localList.isEmpty() && localList.getLast() < DOCUMENT_INDEX)
                        localList.add(DOCUMENT_INDEX);

                }
            }
        } catch (IOException e) {
            System.err.println("Error " + e.getMessage());
        }
    }

    public void indexCollection(String path) {

    }

    private LinkedList<Integer> getIntersection(LinkedList<Integer> firstList, LinkedList<Integer> secondList) {

        return new LinkedList<Integer>();
    }

    private LinkedList<Integer> getUnion(LinkedList<Integer> firstList, LinkedList<Integer> secondList) {

        return new LinkedList<Integer>();
    }

    public LinkedList<Integer> executeQuery(String query) {

        return new LinkedList<Integer>();
    }


}
