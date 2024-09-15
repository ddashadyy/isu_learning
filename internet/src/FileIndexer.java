import java.io.*;
import java.util.*;

public class FileIndexer implements Serializable
{
    private final ArrayList<HashMap<Character, Integer>> percentOfCharactersInFolder = new ArrayList<>();

    private static int countPercentOfCharactersInFile(String file, char character) {
        double fileLength = file.length();
        double characterCounter = 0.0;

        for (int i = 0; i < fileLength; ++i) {
            if (file.charAt(i) == ' ') continue;
            if (file.charAt(i) == character) characterCounter++;
        }

        return (int) (100 * ((characterCounter) / (fileLength)));
    }

    private static HashMap<Character, Integer> indexFile(String fileName) throws FileNotFoundException {
        HashMap<Character, Integer> percentOfCharacters = new HashMap<>();

        try {
            File file = new File(fileName);

            Scanner fileScanner = new Scanner(file);
            String fileContent = fileScanner.nextLine();

            HashSet<Character> uniqueChars = new HashSet<>();
            for (int i = 0; i < fileContent.length(); ++i)
                uniqueChars.add(fileContent.charAt(i));

            for (char character : uniqueChars)
                percentOfCharacters.put(character, countPercentOfCharactersInFile(fileContent, character));

        } catch (FileNotFoundException ex) {
            System.out.println(ex.getMessage());
        }
        return percentOfCharacters;
    }

    public void indexFolder(String folderName) throws FileNotFoundException {
        try {
            File dir = new File(folderName);

            String[] files = dir.list();
            assert files != null;

            for (String file : files)
                percentOfCharactersInFolder.add(indexFile(folderName + '/' + file));
        } catch (FileNotFoundException ex) {
            System.out.println(ex.getMessage());
        }
    }

    private static void serialize(FileIndexer fileIndexer) {
        try {
            ObjectOutputStream objectOutputStream = new ObjectOutputStream(new FileOutputStream("serialize.txt"));
            objectOutputStream.writeObject(fileIndexer);
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    private static void deserialize(String fileName) {
        try {
            ObjectInputStream objectInputStream = new ObjectInputStream(new FileInputStream(fileName));
            FileIndexer fileIndexer = (FileIndexer) objectInputStream.readObject();

            for (HashMap<Character, Integer> map : fileIndexer.percentOfCharactersInFolder)
                map.forEach((key, value) -> System.out.println(key + " " + value));

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    public static void main(String[] args)  {
        try {
            Scanner in = new Scanner(System.in);

            System.out.println("Enter folder name");
            String folderName = in.nextLine();

            FileIndexer fileIndexer = new FileIndexer();
            fileIndexer.indexFolder(folderName);

            for (HashMap<Character, Integer> map : fileIndexer.percentOfCharactersInFolder)
                map.forEach((key, value) -> System.out.println(key + " " + value));

            serialize(fileIndexer);

            deserialize("serialize.txt");

        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }
}