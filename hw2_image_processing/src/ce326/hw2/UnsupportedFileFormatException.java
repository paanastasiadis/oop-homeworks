package ce326.hw2;

public class UnsupportedFileFormatException extends java.lang.Exception {

    public UnsupportedFileFormatException() {}

    public UnsupportedFileFormatException(String correctExtension) {
        super("Not a " + correctExtension + " format.");
    }

}
