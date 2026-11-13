import java.util.Scanner; //있어야 scanner 쓸 수 있음

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        int x = scanner.nextInt();
        int y =  scanner.nextInt();
        int z = scanner.nextInt();

        int result = (x + y) % z;
        System.out.println(result);
    }
}
