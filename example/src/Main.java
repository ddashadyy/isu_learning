import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException {
        InvertedIndex index = new InvertedIndex();

        index.indexDocument("/home/gennadiy/java/example/collection/King_Lear.txt");
        System.out.println(index.index.size());

    }
}