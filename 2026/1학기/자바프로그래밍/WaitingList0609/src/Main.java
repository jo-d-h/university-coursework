import java.sql.SQLException;
import java.util.Scanner;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        MemberService memberService = new MemberService();
        WaitingService waitingService = new WaitingService();
        StatisticsService statsService = new StatisticsService();
        Scanner sc = new Scanner(System.in);

        System.out.println("=== 대기 관리 시스템 ===");

        while (true) {
            System.out.println("1. 로그인");
            System.out.println("2. 회원가입");
            System.out.println("3. 종료");
            System.out.print("선택: ");

            String choice = sc.nextLine();

            if (choice.equals("1")) {
                MemberService.MemberResult result = handleLogin(sc, memberService);

                if (result != null && result.isSuccess()) {
                    if (result.getMember().isAdmin) {
                        runAdminMenu(sc, waitingService, statsService);
                    } else {
                        runUserMenu(sc, waitingService, result.getMember());
                    }
                }
            } else if (choice.equals("2")) {
                handleSignUp(sc, memberService);
            } else if (choice.equals("3")) {
                System.out.println("프로그램을 종료합니다.");
                break;
            } else {
                System.out.println("잘못된 선택입니다.");
            }
        }
        sc.close();
    }

    private static void runUserMenu(Scanner sc, WaitingService service, Member member) {
        while (true) {
            printUserMenu();
            int choice = inputInt(sc, "선택: ");

            switch (choice) {
                case 1:
                    int count = inputInt(sc, "인원수 입력: ");
                    System.out.println(service.registerWaiting(member.memberId, member.name, member.phone, count).getMessage());
                    break;
                case 2:
                    showMyWaitingStatus(service, member);
                    break;
                case 3:
                    cancelMyWaiting(sc, service, member);
                    break;
                case 4:
                    System.out.println("로그아웃합니다.");
                    return;
                default:
                    System.out.println("잘못된 선택입니다.");
            }
        }
    }

    private static void runAdminMenu(Scanner sc, WaitingService service, StatisticsService statsService) {
        while (true) {
            printAdminMenu();
            System.out.print("선택: ");
            int choice = inputInt(sc, "");

            switch (choice) {
                case 1: showWaitingList(service); break;
                case 2: callNextCustomer(service); break;
                case 3: enterCustomer(sc, service); break;
                case 4: cancelWaiting(sc, service); break;
                case 5: noShowWaiting(sc, service); break;
                case 6: changeStatus(sc, service); break;
                case 7: showStatistics(statsService); break;
                case 8: System.out.println("관리자 메뉴를 종료합니다."); return;
                default: System.out.println("잘못된 메뉴입니다.");
            }
        }
    }

    private static void handleSignUp(Scanner sc, MemberService service) {
        System.out.println("\n=== 회원가입 ===");

        // 아이디 입력 및 검증 (4~20자, 영문/숫자만)
        String id;
        while (true) {
            System.out.print("아이디 (4~20자, 영문/숫자만): ");
            id = sc.nextLine().trim();
            if (id.length() < 4 || id.length() > 20) {
                System.out.println("아이디는 4자 이상 20자 이하로 입력해주세요.");
            } else if (!id.matches("[a-zA-Z0-9]+")) {
                System.out.println("아이디는 영문과 숫자만 사용할 수 있습니다.");
            } else {
                break;
            }
        }

        // 비밀번호 입력 및 검증 (8~20자, 영문/숫자만)
        String pw;
        while (true) {
            System.out.print("비밀번호 (8~20자, 영문/숫자만): ");
            pw = sc.nextLine().trim();
            if (pw.length() < 8 || pw.length() > 20) {
                System.out.println("비밀번호는 8자 이상 20자 이하로 입력해주세요.");
            } else if (!pw.matches("[a-zA-Z0-9]+")) {
                System.out.println("비밀번호는 영문과 숫자만 사용할 수 있습니다.");
            } else {
                break;
            }
        }

        // 이름 입력 및 검증 (한글/영문만, 특수문자 X)
        String name;
        while (true) {
            System.out.print("이름 (한글/영문만): ");
            name = sc.nextLine().trim();
            if (name.isEmpty()) {
                System.out.println("이름을 입력해주세요.");
            } else if (!name.matches("[a-zA-Z가-힣]+")) {
                System.out.println("이름은 한글 또는 영문만 입력할 수 있습니다.");
            } else {
                break;
            }
        }

        // 전화번호 입력 및 검증 (010-XXXX-XXXX 형식)
        String phone;
        while (true) {
            System.out.print("전화번호 (010-XXXX-XXXX): ");
            phone = sc.nextLine().trim();
            if (!phone.matches("010-\\d{4}-\\d{4}")) {
                System.out.println("전화번호 형식이 올바르지 않습니다. (예: 010-1234-5678)");
            } else {
                break;
            }
        }

        MemberService.MemberResult result = service.signUp(name, phone, id, pw, false);
        System.out.println(result.getMessage());
    }

    private static MemberService.MemberResult handleLogin(Scanner sc, MemberService service) {
        System.out.println("\n=== 로그인 ===");
        System.out.print("아이디: ");
        String id = sc.nextLine().trim();
        System.out.print("비밀번호: ");
        String pw = sc.nextLine().trim();

        MemberService.MemberResult result = service.login(id, pw);

        if (result.isSuccess()) {
            Member member = result.getMember();
            if (member.isAdmin) {
                System.out.println(member.name + "님 환영합니다! [관리자 계정]");
            } else {
                System.out.println(member.name + "님 환영합니다!");
            }
            return result;
        } else {
            System.out.println("로그인 실패: " + result.getMessage());
            return null;
        }
    }

    private static void printAdminMenu() {
        System.out.println("\n===== 관리자 대기 관리 시스템 =====");
        System.out.println("1. 대기 목록 조회 | 2. 다음 고객 호출 | 3. 입장 처리 | 4. 대기 취소");
        System.out.println("5. 노쇼 처리 | 6. 상태 변경 | 7. 통계 보기 | 8. 종료");
    }

    private static void printUserMenu() {
        System.out.println("\n===== 고객 전용 메뉴 =====");
        System.out.println("1. 대기 등록하기");
        System.out.println("2. 내 대기 상태 확인");
        System.out.println("3. 대기 취소");
        System.out.println("4. 로그아웃");
    }

    private static int inputInt(Scanner sc, String message) {
        while (true) {
            System.out.print(message);
            String input = sc.nextLine().trim();
            try {
                return Integer.parseInt(input);
            } catch (NumberFormatException e) {
                System.out.println("숫자만 입력하세요.");
            }
        }
    }

    // ===== 고객 전용 메서드 =====

    // 내 대기 상태 확인 - memberId 기반으로 조회
    private static void showMyWaitingStatus(WaitingService service, Member member) {
        try {
            List<Waiting> list = service.getWaitingListByMemberId(member.memberId);

            if (list.isEmpty()) {
                System.out.println("현재 등록된 대기가 없습니다.");
                return;
            }

            for (Waiting w : list) {
                System.out.println("내 대기 정보: " + w);
            }

            int position = service.getWaitingPosition(member.memberId);
            if (position > 0) {
                System.out.println("현재 순번: " + position + "번째");
            }
        } catch (SQLException e) {
            System.out.println("조회 중 오류 발생");
        }
    }

    // 내 대기 취소 - memberId + waitingNum으로 본인 것만 취소
    private static void cancelMyWaiting(Scanner sc, WaitingService service, Member member) {
        try {
            List<Waiting> list = service.getWaitingListByMemberId(member.memberId);

            if (list.isEmpty()) {
                System.out.println("취소할 대기가 없습니다.");
                return;
            }

            for (Waiting w : list) {
                System.out.println("내 대기 정보: " + w);
            }

            int waitingNum = inputInt(sc, "취소할 대기번호 입력: ");
            System.out.println(service.cancelMyWaiting(member.memberId, waitingNum).getMessage());
        } catch (SQLException e) {
            System.out.println("취소 처리 중 오류 발생");
        }
    }

    // ===== 관리자 전용 메서드 =====

    private static void cancelWaiting(Scanner sc, WaitingService service) {
        if (!showWaitingList(service)) return;
        int num = inputInt(sc, "취소할 대기번호 입력: ");
        System.out.println(service.cancelWaitingFromDB(num).getMessage());
    }

    private static void callNextCustomer(WaitingService service) {
        System.out.println(service.callNextWaiting().getMessage());
    }

    private static void enterCustomer(Scanner sc, WaitingService service) {
        if (!showCallList(service)) return;
        int num = inputInt(sc, "입장 처리할 대기번호 입력: ");
        System.out.println(service.enterWaiting(num).getMessage());
    }

    private static void noShowWaiting(Scanner sc, WaitingService service) {
        if (!showCallList(service)) return;
        int num = inputInt(sc, "노쇼 처리할 대기번호 입력: ");
        System.out.println(service.noShowWaitingFromDB(num).getMessage());
    }

    private static void changeStatus(Scanner sc, WaitingService service) {
        System.out.println("현재 상태: " + service.getStoreStatus());
        System.out.println("1. OPEN | 2. PAUSED | 3. CLOSED");
        int choice = inputInt(sc, "선택: ");
        String[] status = {"OPEN", "PAUSED", "CLOSED"};
        if (choice >= 1 && choice <= 3) {
            System.out.println(service.changeStoreStatusForDisplay(status[choice - 1]).getMessage());
        } else {
            System.out.println("잘못된 선택입니다.");
        }
    }

    private static boolean showWaitingList(WaitingService service) {
        System.out.println("\n<현재 대기 목록>");
        try {
            List<Waiting> list = service.getActiveWaitingList();
            if (list.isEmpty()) {
                System.out.println("대기 고객이 없습니다.");
                return false;
            }
            list.forEach(System.out::println);
            return true;
        } catch (SQLException e) {
            return false;
        }
    }

    private static boolean showCallList(WaitingService service) {
        System.out.println("\n<현재 호출 목록>");
        try {
            List<Waiting> list = service.getCallWaitingList();
            if (list.isEmpty()) {
                System.out.println("호출 중인 고객이 없습니다.");
                return false;
            }
            list.forEach(System.out::println);
            return true;
        } catch (SQLException e) {
            return false;
        }
    }

    private static void showStatistics(StatisticsService statsService) {
        try {
            System.out.println("\n===== 통계 정보 =====");
            System.out.println("총 대기: " + statsService.getTotalWaitingCount());
            System.out.println("입장 완료: " + statsService.getEnteredCount());
            System.out.println("취소: " + statsService.getCancelCount());
            System.out.println("노쇼: " + statsService.getNoShowCount());
            System.out.println("현재 혼잡도: " + statsService.getBusyLevel());
        } catch (SQLException e) {
            System.out.println("통계 조회 오류");
        }
    }
}