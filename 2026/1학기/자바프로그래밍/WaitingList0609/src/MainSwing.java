import javax.swing.*;
import java.awt.*;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.sql.SQLException;
import java.util.List;

public class MainSwing extends JFrame {

    private JTextField idField;
    private JPasswordField passwordField;
    private JButton loginButton;
    private JButton signUpButton;

    private JTextField signUpNameField;
    private JTextField signUpIdField;
    private JPasswordField signUpPasswordField;
    private JTextField signUpPhoneField;
    private JButton joinButton;
    private JButton backButton;

    private MemberService memberService = new MemberService();
    private WaitingService waitingService = new WaitingService();
    private Member loginMember;

    public MainSwing() {
        setTitle("WaitingList");
        setSize(800, 700);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        showLoginPanel();

        setVisible(true);
    }

    private void showLoginPanel() {
        setContentPane(createLoginPanel());
        revalidate();
        repaint();
    }

    private void showSignUpPanel() {
        setContentPane(createSignUpPanel());
        revalidate();
        repaint();
    }

    private JPanel createLoginPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(70, 60, 70, 60));

        JLabel titleLabel = new JLabel("WaitingList");
        titleLabel.setFont(new Font("Arial", Font.BOLD, 38));
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.Y_AXIS));

        JLabel idLabel = new JLabel("아이디");
        idLabel.setFont(new Font("맑은 고딕", Font.BOLD, 16));

        idField = new JTextField();
        idField.setFont(new Font("맑은 고딕", Font.PLAIN, 18));
        idField.setMaximumSize(new Dimension(Integer.MAX_VALUE, 45));

        JLabel passwordLabel = new JLabel("비밀번호");
        passwordLabel.setFont(new Font("맑은 고딕", Font.BOLD, 16));

        passwordField = new JPasswordField();
        passwordField.setFont(new Font("맑은 고딕", Font.PLAIN, 18));
        passwordField.setMaximumSize(new Dimension(Integer.MAX_VALUE, 45));

        loginButton = new JButton("로그인");
        loginButton.setFont(new Font("맑은 고딕", Font.BOLD, 17));
        loginButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 48));

        loginButton.addActionListener(e -> {
            String loginId = idField.getText().trim();
            String password = new String(passwordField.getPassword()).trim();

            MemberService.MemberResult result = memberService.login(loginId, password);

            JOptionPane.showMessageDialog(this, result.getMessage());

            if (result.isSuccess()) {
                loginMember = result.getMember();

                if (loginMember.isAdmin) {
                    showAdminMainPanel();
                } else {
                    showUserMainPanel();
                }
            }
        });

        signUpButton = new JButton("회원가입");
        signUpButton.setFont(new Font("맑은 고딕", Font.BOLD, 17));
        signUpButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 48));

        signUpButton.addActionListener(e -> showSignUpPanel());

        centerPanel.add(Box.createVerticalStrut(45));
        centerPanel.add(idLabel);
        centerPanel.add(Box.createVerticalStrut(8));
        centerPanel.add(idField);
        centerPanel.add(Box.createVerticalStrut(25));
        centerPanel.add(passwordLabel);
        centerPanel.add(Box.createVerticalStrut(8));
        centerPanel.add(passwordField);
        centerPanel.add(Box.createVerticalStrut(35));
        centerPanel.add(loginButton);
        centerPanel.add(Box.createVerticalStrut(16));
        centerPanel.add(signUpButton);

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(centerPanel, BorderLayout.CENTER);

        return panel;
    }

    private void showAdminMainPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(60, 60, 60, 60));

        JLabel titleLabel = new JLabel("WaitingList 관리 시스템");
        titleLabel.setFont(new Font("맑은 고딕", Font.BOLD, 30));
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));

        JButton listStatusButton = new JButton("조회 및 상태변경");
        JButton noShowButton = new JButton("노쇼처리");
        JButton waitingButton = new JButton("웨이팅");
        JButton statisticsButton = new JButton("통계보기");
        JButton exitButton = new JButton("종료");

        listStatusButton.addActionListener(e -> showAdminListStatusPanel());

        noShowButton.addActionListener(e -> showNoShowPanel());

        Font buttonFont = new Font("맑은 고딕", Font.BOLD, 18);

        listStatusButton.setFont(buttonFont);
        noShowButton.setFont(buttonFont);
        waitingButton.setFont(buttonFont);
        statisticsButton.setFont(buttonFont);
        exitButton.setFont(buttonFont);

        listStatusButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));
        noShowButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));
        waitingButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));
        statisticsButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));
        exitButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));

        listStatusButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        noShowButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        waitingButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        statisticsButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        exitButton.setAlignmentX(Component.CENTER_ALIGNMENT);

        exitButton.addActionListener(e -> showLoginPanel());
        waitingButton.addActionListener(e -> showWaitingStatusDialog());
        statisticsButton.addActionListener(e -> showStatisticsPanel());

        buttonPanel.add(Box.createVerticalStrut(40));
        buttonPanel.add(listStatusButton);
        buttonPanel.add(Box.createVerticalStrut(15));
        buttonPanel.add(noShowButton);
        buttonPanel.add(Box.createVerticalStrut(15));
        buttonPanel.add(waitingButton);
        buttonPanel.add(Box.createVerticalStrut(15));
        buttonPanel.add(statisticsButton);
        buttonPanel.add(Box.createVerticalStrut(15));
        buttonPanel.add(exitButton);

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(buttonPanel, BorderLayout.CENTER);

        setContentPane(panel);
        revalidate();
        repaint();
    }

    private void showWaitingStatusDialog() {
        JDialog dialog = new JDialog(this, "웨이팅 상태 변경", true);
        dialog.setSize(360, 260);
        dialog.setLocationRelativeTo(this);
        dialog.setLayout(new BorderLayout(15, 15));

        JPanel panel = new JPanel();
        panel.setBorder(BorderFactory.createEmptyBorder(20, 25, 20, 25));
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));

        JLabel titleLabel = new JLabel("웨이팅 상태 변경");
        titleLabel.setFont(new Font("맑은 고딕", Font.BOLD, 20));
        titleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel currentLabel = new JLabel("현재 상태: " + waitingService.getStoreStatus());
        currentLabel.setFont(new Font("맑은 고딕", Font.PLAIN, 15));
        currentLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel selectLabel = new JLabel("변경할 상태 선택");
        selectLabel.setFont(new Font("맑은 고딕", Font.PLAIN, 15));
        selectLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JComboBox<String> statusComboBox = new JComboBox<>(new String[]{"OPEN", "PAUSED", "CLOSED"});
        statusComboBox.setMaximumSize(new Dimension(Integer.MAX_VALUE, 35));
        statusComboBox.setFont(new Font("맑은 고딕", Font.PLAIN, 15));

        JButton changeButton = new JButton("변경");
        changeButton.setFont(new Font("맑은 고딕", Font.BOLD, 15));
        changeButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 38));

        JButton closeButton = new JButton("닫기");
        closeButton.setFont(new Font("맑은 고딕", Font.BOLD, 15));
        closeButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 38));

        changeButton.addActionListener(e -> {
            String selectedStatus = (String) statusComboBox.getSelectedItem();

            WaitingService.OperationResult result =
                    waitingService.changeStoreStatusForDisplay(selectedStatus);

            JOptionPane.showMessageDialog(dialog, result.getMessage());

            if (result.isSuccess()) {
                currentLabel.setText("현재 상태: " + waitingService.getStoreStatus());
            }
        });

        closeButton.addActionListener(e -> dialog.dispose());

        panel.add(titleLabel);
        panel.add(Box.createVerticalStrut(20));
        panel.add(currentLabel);
        panel.add(Box.createVerticalStrut(15));
        panel.add(selectLabel);
        panel.add(Box.createVerticalStrut(8));
        panel.add(statusComboBox);
        panel.add(Box.createVerticalStrut(18));
        panel.add(changeButton);
        panel.add(Box.createVerticalStrut(10));
        panel.add(closeButton);

        dialog.add(panel, BorderLayout.CENTER);
        dialog.setVisible(true);
    }

    private void showAdminListStatusPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(35, 40, 35, 40));

        JLabel titleLabel = new JLabel("조회 및 상태변경");
        titleLabel.setFont(new Font("맑은 고딕", Font.BOLD, 28));
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        DefaultListModel<Waiting> listModel = new DefaultListModel<>();
        JList<Waiting> waitingListView = new JList<>(listModel);
        waitingListView.setFont(new Font("맑은 고딕", Font.PLAIN, 15));

        try {
            List<Waiting> waitingList = waitingService.getAllWaitingList();

            for (Waiting waiting : waitingList) {
                listModel.addElement(waiting);
            }
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(this, "고객 목록 조회 중 오류가 발생했습니다.\n" + e.getMessage());
        }

        JScrollPane scrollPane = new JScrollPane(waitingListView);

        JPanel bottomPanel = new JPanel(new GridBagLayout());
        bottomPanel.setBorder(BorderFactory.createEmptyBorder(15, 0, 0, 0));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(6, 5, 6, 5);
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weighty = 1.0;

        JButton callButton = new JButton("호출");
        JButton enterButton = new JButton("입장");
        JButton backButton = new JButton("뒤로가기");
        JButton cancelButton = new JButton("취소");

        JTextField enterField = new JTextField("입장할 번호를 입력하세요");
        enterField.setForeground(Color.GRAY);

        JTextField cancelField = new JTextField("취소할 번호를 입력하세요");
        cancelField.setForeground(Color.GRAY);

        FocusAdapter enterPlaceholder = new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent e) {
                if (enterField.getText().equals("입장할 번호를 입력하세요")) {
                    enterField.setText("");
                    enterField.setForeground(Color.BLACK);
                }
            }

            @Override
            public void focusLost(FocusEvent e) {
                if (enterField.getText().trim().isEmpty()) {
                    enterField.setText("입장할 번호를 입력하세요");
                    enterField.setForeground(Color.GRAY);
                }
            }
        };

        FocusAdapter cancelPlaceholder = new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent e) {
                if (cancelField.getText().equals("취소할 번호를 입력하세요")) {
                    cancelField.setText("");
                    cancelField.setForeground(Color.BLACK);
                }
            }

            @Override
            public void focusLost(FocusEvent e) {
                if (cancelField.getText().trim().isEmpty()) {
                    cancelField.setText("취소할 번호를 입력하세요");
                    cancelField.setForeground(Color.GRAY);
                }
            }
        };

        enterField.addFocusListener(enterPlaceholder);
        cancelField.addFocusListener(cancelPlaceholder);

        Font buttonFont = new Font("맑은 고딕", Font.BOLD, 16);
        Font fieldFont = new Font("맑은 고딕", Font.PLAIN, 15);

        callButton.setFont(buttonFont);
        enterButton.setFont(buttonFont);
        backButton.setFont(buttonFont);
        cancelButton.setFont(buttonFont);
        enterField.setFont(fieldFont);
        cancelField.setFont(fieldFont);

        callButton.addActionListener(e -> {
            WaitingService.OperationResult result = waitingService.callNextWaiting();

            if (!result.isSuccess()) {
                JOptionPane.showMessageDialog(this, "호출할 고객이 없습니다.");
                return;
            }

            JOptionPane.showMessageDialog(this, result.getMessage());
            showAdminListStatusPanel();
        });

        enterButton.addActionListener(e -> {
            String input = enterField.getText().trim();

            if (input.isEmpty() || input.equals("입장할 번호를 입력하세요")) {
                JOptionPane.showMessageDialog(this, "입장할 번호를 입력하세요.");
                return;
            }

            try {
                int waitingNum = Integer.parseInt(input);

                WaitingService.OperationResult result = waitingService.enterWaiting(waitingNum);

                if (!result.isSuccess()) {
                    JOptionPane.showMessageDialog(this, "고객을 먼저 호출하세요.");
                    return;
                }

                JOptionPane.showMessageDialog(this, result.getMessage());
                showAdminListStatusPanel();

            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(this, "숫자만 입력하세요.");
            }
        });

        cancelButton.addActionListener(e -> {
            String input = cancelField.getText().trim();

            if (input.isEmpty() || input.equals("취소할 번호를 입력하세요")) {
                JOptionPane.showMessageDialog(this, "취소할 번호를 입력하세요.");
                return;
            }

            try {
                int waitingNum = Integer.parseInt(input);

                WaitingService.OperationResult result = waitingService.cancelWaitingFromDB(waitingNum);

                if (!result.isSuccess()) {
                    if (result.getMessage().contains("이미 취소")) {
                        JOptionPane.showMessageDialog(this, "이미 취소된 고객입니다.");
                    } else {
                        JOptionPane.showMessageDialog(this, result.getMessage());
                    }
                    return;
                }

                JOptionPane.showMessageDialog(this, result.getMessage());
                showAdminListStatusPanel();

            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(this, "숫자만 입력하세요.");
            }
        });

        backButton.addActionListener(e -> showAdminMainPanel());

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 0.25;
        bottomPanel.add(callButton, gbc);

        gbc.gridx = 1;
        gbc.gridy = 0;
        gbc.weightx = 0.55;
        bottomPanel.add(enterField, gbc);

        gbc.gridx = 2;
        gbc.gridy = 0;
        gbc.weightx = 0.20;
        bottomPanel.add(enterButton, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.weightx = 0.25;
        bottomPanel.add(backButton, gbc);

        gbc.gridx = 1;
        gbc.gridy = 1;
        gbc.weightx = 0.55;
        bottomPanel.add(cancelField, gbc);

        gbc.gridx = 2;
        gbc.gridy = 1;
        gbc.weightx = 0.20;
        bottomPanel.add(cancelButton, gbc);

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(bottomPanel, BorderLayout.SOUTH);

        setContentPane(panel);
        revalidate();
        repaint();
    }

    private void showNoShowPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(35, 40, 35, 40));

        JLabel titleLabel = new JLabel("노쇼 처리");
        titleLabel.setFont(new Font("맑은 고딕", Font.BOLD, 28));
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        DefaultListModel<String> listModel = new DefaultListModel<>();
        JList<String> callListView = new JList<>(listModel);
        callListView.setFont(new Font("맑은 고딕", Font.PLAIN, 15));

        try {
            List<Waiting> callList = waitingService.getCallWaitingList();

            if (callList.isEmpty()) {
                listModel.addElement("CALL 상태인 고객이 없습니다.");
            } else {


                for (Waiting waiting : callList) {
                    long passedMinutes = waitingService.getPassedMinutes(waiting.calledAt);

                    listModel.addElement(
                            String.format("%03d | %s | %s | %s | %d분",
                                    waiting.waitingNum,
                                    waiting.name,
                                    waiting.phone,
                                    waiting.status,
                                    passedMinutes)
                    );
                }
            }
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(this, "CALL 목록 조회 중 오류가 발생했습니다.\n" + e.getMessage());
        }

        JScrollPane scrollPane = new JScrollPane(callListView);

        JPanel bottomPanel = new JPanel();
        bottomPanel.setLayout(new BoxLayout(bottomPanel, BoxLayout.Y_AXIS));

        JTextField noShowField = new JTextField("노쇼 처리할 대기번호를 입력하세요");
        noShowField.setFont(new Font("맑은 고딕", Font.PLAIN, 15));
        noShowField.setForeground(Color.GRAY);
        noShowField.setMaximumSize(new Dimension(Integer.MAX_VALUE, 40));

        noShowField.addFocusListener(new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent e) {
                if (noShowField.getText().equals("노쇼 처리할 대기번호를 입력하세요")) {
                    noShowField.setText("");
                    noShowField.setForeground(Color.BLACK);
                }
            }

            @Override
            public void focusLost(FocusEvent e) {
                if (noShowField.getText().trim().isEmpty()) {
                    noShowField.setText("노쇼 처리할 대기번호를 입력하세요");
                    noShowField.setForeground(Color.GRAY);
                }
            }
        });

        JButton noShowProcessButton = new JButton("노쇼 처리");
        noShowProcessButton.setFont(new Font("맑은 고딕", Font.BOLD, 16));
        noShowProcessButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 45));

        JButton backButton = new JButton("뒤로가기");
        backButton.setFont(new Font("맑은 고딕", Font.BOLD, 16));
        backButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 45));

        noShowProcessButton.addActionListener(e -> {
            String input = noShowField.getText().trim();

            if (input.isEmpty() || input.equals("노쇼 처리할 대기번호를 입력하세요")) {
                JOptionPane.showMessageDialog(this, "노쇼 처리할 대기번호를 입력하세요.");
                return;
            }

            try {
                int waitingNum = Integer.parseInt(input);

                WaitingService.OperationResult result =
                        waitingService.noShowWaitingFromDB(waitingNum);

                JOptionPane.showMessageDialog(this, result.getMessage());

                if (result.isSuccess()) {
                    showNoShowPanel();
                }
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(this, "숫자만 입력하세요.");
            }
        });

        backButton.addActionListener(e -> showAdminMainPanel());

        bottomPanel.add(Box.createVerticalStrut(15));
        bottomPanel.add(noShowField);
        bottomPanel.add(Box.createVerticalStrut(12));
        bottomPanel.add(noShowProcessButton);
        bottomPanel.add(Box.createVerticalStrut(12));
        bottomPanel.add(backButton);

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(bottomPanel, BorderLayout.SOUTH);

        setContentPane(panel);
        revalidate();
        repaint();
    }

    private void showUserMainPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(60, 60, 60, 60));

        JLabel titleLabel = new JLabel("WaitingList");
        titleLabel.setFont(new Font("Arial", Font.BOLD, 38));
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));

        JButton waitingRegisterButton = new JButton("대기등록");
        waitingRegisterButton.addActionListener(
                e -> showWaitingRegisterPanel()
        );
        JButton checkOrderButton = new JButton("순번확인");
        checkOrderButton.addActionListener(e -> showCheckOrderPanel());
        JButton exitButton = new JButton("종료");

        Font buttonFont = new Font("맑은 고딕", Font.BOLD, 18);

        waitingRegisterButton.setFont(buttonFont);
        checkOrderButton.setFont(buttonFont);
        exitButton.setFont(buttonFont);

        waitingRegisterButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));
        checkOrderButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));
        exitButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 60));

        buttonPanel.add(Box.createVerticalStrut(40));
        buttonPanel.add(waitingRegisterButton);
        buttonPanel.add(Box.createVerticalStrut(20));
        buttonPanel.add(checkOrderButton);
        buttonPanel.add(Box.createVerticalStrut(20));
        buttonPanel.add(exitButton);

        waitingRegisterButton.addActionListener(e -> showWaitingRegisterPanel());
        exitButton.addActionListener(e -> showLoginPanel());

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(buttonPanel, BorderLayout.CENTER);

        setContentPane(panel);
        revalidate();
        repaint();
    }

    private void showStatisticsPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 15));

        JLabel titleLabel = new JLabel("통계 보기");
        titleLabel.setFont(new Font("맑은 고딕", Font.BOLD, 20));
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel contentPanel = new JPanel();
        contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.Y_AXIS));

        JLabel totalTitleLabel = new JLabel("누적 통계");
        totalTitleLabel.setFont(new Font("맑은 고딕", Font.BOLD, 14));
        totalTitleLabel.setAlignmentX(Component.LEFT_ALIGNMENT);

        JPanel cardPanel = new JPanel(new GridLayout(2, 2, 8, 8));
        cardPanel.setMaximumSize(new Dimension(Integer.MAX_VALUE, 140));
        cardPanel.setAlignmentX(Component.LEFT_ALIGNMENT);

        JPanel totalWaitingCard = createStatCard("총 대기 등록", "0");
        JPanel enteredCard = createStatCard("입장 완료", "0");
        JPanel cancelCard = createStatCard("취소", "0");
        JPanel noShowCard = createStatCard("노쇼", "0");

        int totalWaitingCount = 0;
        int enteredCount = 0;
        int cancelCount = 0;
        int noShowCount = 0;
        int activeWaitingCount = 0;
        int maxWaitingCount = 0;
        int busyRatio = 0;

        try {
            StatisticsService statisticsService = new StatisticsService();
            StatisticsService.StatisticsSummary summary = statisticsService.getStatisticsSummary();

            totalWaitingCount = summary.totalWaitingCount;
            enteredCount = summary.enteredCount;
            cancelCount = summary.cancelCount;
            noShowCount = summary.noShowCount;
            activeWaitingCount = summary.activeWaitingCount;

            maxWaitingCount = summary.maxWaitingCount;
            busyRatio = summary.busyRatio;

        } catch (SQLException e) {
            JOptionPane.showMessageDialog(this, "통계 조회 중 오류가 발생했습니다.\n" + e.getMessage());
        }

        updateStatCard(totalWaitingCard, "총 대기 등록", String.valueOf(totalWaitingCount));
        updateStatCard(enteredCard, "입장 완료", String.valueOf(enteredCount));
        updateStatCard(cancelCard, "취소", String.valueOf(cancelCount));
        updateStatCard(noShowCard, "노쇼", String.valueOf(noShowCount));

        cardPanel.add(totalWaitingCard);
        cardPanel.add(enteredCard);
        cardPanel.add(cancelCard);
        cardPanel.add(noShowCard);

        JPanel busyPanel = new JPanel();
        busyPanel.setLayout(new BoxLayout(busyPanel, BoxLayout.Y_AXIS));
        busyPanel.setBorder(BorderFactory.createTitledBorder("혼잡도 예측"));
        busyPanel.setMaximumSize(new Dimension(Integer.MAX_VALUE, 110));
        busyPanel.setAlignmentX(Component.LEFT_ALIGNMENT);

        JProgressBar currentBar = new JProgressBar(0, 100);
        currentBar.setValue(busyRatio);
        currentBar.setStringPainted(false);

        JProgressBar maxBar = new JProgressBar(0, 100);
        maxBar.setValue(100);
        maxBar.setStringPainted(false);

        JPanel currentRow = new JPanel(new BorderLayout(8, 0));
        currentRow.add(new JLabel("현재 대기"), BorderLayout.WEST);
        currentRow.add(currentBar, BorderLayout.CENTER);
        currentRow.add(new JLabel(activeWaitingCount + "팀"), BorderLayout.EAST);

        JPanel maxRow = new JPanel(new BorderLayout(8, 0));
        maxRow.add(new JLabel("최대 대기"), BorderLayout.WEST);
        maxRow.add(maxBar, BorderLayout.CENTER);
        maxRow.add(new JLabel(maxWaitingCount + "팀"), BorderLayout.EAST);

        JLabel ratioLabel = new JLabel(
                "현재대기/최대대기 = "
                        + activeWaitingCount + "/"
                        + maxWaitingCount + " = "
                        + busyRatio + "%"
        );
        ratioLabel.setHorizontalAlignment(SwingConstants.CENTER);

        JLabel busyLevelLabel = new JLabel();
        busyLevelLabel.setOpaque(true);
        busyLevelLabel.setHorizontalAlignment(SwingConstants.CENTER);
        busyLevelLabel.setPreferredSize(new Dimension(55, 28));

        if (busyRatio < 30) {
            busyLevelLabel.setText("낮음");
            busyLevelLabel.setBackground(new Color(120, 180, 240));
        } else if (busyRatio <= 70) {
            busyLevelLabel.setText("보통");
            busyLevelLabel.setBackground(new Color(240, 200, 80));
        } else {
            busyLevelLabel.setText("높음");
            busyLevelLabel.setBackground(new Color(220, 70, 70));
        }

        JLabel guideLabel = new JLabel("낮음<30%   보통30%-70%   높음>70%");
        guideLabel.setFont(new Font("맑은 고딕", Font.PLAIN, 11));

        JPanel levelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        levelPanel.add(busyLevelLabel);
        levelPanel.add(guideLabel);

        busyPanel.add(currentRow);
        busyPanel.add(Box.createVerticalStrut(4));
        busyPanel.add(maxRow);
        busyPanel.add(Box.createVerticalStrut(5));
        busyPanel.add(ratioLabel);
        busyPanel.add(levelPanel);

        JButton backButton = new JButton("뒤로가기");
        backButton.setFont(new Font("맑은 고딕", Font.BOLD, 16));
        backButton.setPreferredSize(new Dimension(Integer.MAX_VALUE, 45));
        backButton.addActionListener(e -> showAdminMainPanel());

        contentPanel.add(Box.createVerticalStrut(15));
        contentPanel.add(totalTitleLabel);
        contentPanel.add(Box.createVerticalStrut(8));
        contentPanel.add(cardPanel);
        contentPanel.add(Box.createVerticalStrut(10));
        contentPanel.add(busyPanel);

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(contentPanel, BorderLayout.CENTER);
        panel.add(backButton, BorderLayout.SOUTH);

        setContentPane(panel);
        revalidate();
        repaint();
    }
    private JPanel createStatCard(String title, String value) {
        JPanel card = new JPanel(new BorderLayout());
        card.setBorder(BorderFactory.createLineBorder(Color.LIGHT_GRAY));
        card.setBackground(Color.WHITE);

        JLabel titleLabel = new JLabel(title);
        titleLabel.setFont(new Font("맑은 고딕", Font.PLAIN, 13));
        titleLabel.setBorder(BorderFactory.createEmptyBorder(8, 10, 0, 10));

        JLabel valueLabel = new JLabel(value);
        valueLabel.setFont(new Font("맑은 고딕", Font.BOLD, 24));
        valueLabel.setBorder(BorderFactory.createEmptyBorder(0, 10, 8, 10));

        card.add(titleLabel, BorderLayout.NORTH);
        card.add(valueLabel, BorderLayout.CENTER);

        return card;
    }

    private void updateStatCard(JPanel card, String title, String value) {
        card.removeAll();

        JLabel titleLabel = new JLabel(title);
        titleLabel.setFont(new Font("맑은 고딕", Font.PLAIN, 13));
        titleLabel.setBorder(BorderFactory.createEmptyBorder(8, 10, 0, 10));

        JLabel valueLabel = new JLabel(value);
        valueLabel.setFont(new Font("맑은 고딕", Font.BOLD, 24));
        valueLabel.setBorder(BorderFactory.createEmptyBorder(0, 10, 8, 10));

        card.add(titleLabel, BorderLayout.NORTH);
        card.add(valueLabel, BorderLayout.CENTER);
    }

    private void showCheckOrderPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(60, 60, 60, 60));

        JLabel titleLabel = new JLabel("순번 확인");
        titleLabel.setFont(new Font("맑은 고딕", Font.BOLD, 32));
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.Y_AXIS));

        JLabel guideLabel = new JLabel("고객님의 현재 순번은");
        guideLabel.setFont(new Font("맑은 고딕", Font.BOLD, 22));
        guideLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel orderLabel = new JLabel();
        orderLabel.setFont(new Font("맑은 고딕", Font.BOLD, 48));
        orderLabel.setForeground(new Color(40, 90, 180));
        orderLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel endLabel = new JLabel("입니다");
        endLabel.setFont(new Font("맑은 고딕", Font.BOLD, 22));
        endLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JButton cancelButton = new JButton("대기취소");
        cancelButton.setFont(new Font("맑은 고딕", Font.BOLD, 18));

        JButton backButton = new JButton("뒤로가기");
        backButton.setFont(new Font("맑은 고딕", Font.BOLD, 18));

        // GridLayout으로 버튼 패널 구성 → 자동으로 가로 꽉 채움
        JPanel buttonPanel = new JPanel(new GridLayout(2, 1, 0, 15));
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(20, 0, 0, 0));
        buttonPanel.add(cancelButton);
        buttonPanel.add(backButton);

        int position = 0;

        try {
            if (loginMember == null) {
                JOptionPane.showMessageDialog(this, "로그인 정보가 없습니다. 다시 로그인해주세요.");
                showLoginPanel();
                return;
            }

            position = waitingService.getWaitingPosition(loginMember.memberId);

            if (position == -1) {
                orderLabel.setText("호출");
                orderLabel.setForeground(new Color(220, 70, 70));
                cancelButton.setEnabled(true);
            } else if (position == 0) {
                orderLabel.setText("대기 없음");
                cancelButton.setEnabled(false);
            } else {
                orderLabel.setText(position + "번");
                orderLabel.setForeground(new Color(40, 90, 180));
                cancelButton.setEnabled(true);
            }

        } catch (SQLException e) {
            JOptionPane.showMessageDialog(this, "순번 조회 중 오류가 발생했습니다.\n" + e.getMessage());
            orderLabel.setText("조회 실패");
            cancelButton.setEnabled(false);
        }

        cancelButton.addActionListener(e -> {
            if (loginMember == null) {
                JOptionPane.showMessageDialog(this, "로그인 정보가 없습니다. 다시 로그인해주세요.");
                showLoginPanel();
                return;
            }

            try {
                List<Waiting> myWaitingList = waitingService.getWaitingListByMemberId(loginMember.memberId);

                Waiting target = null;

                for (Waiting waiting : myWaitingList) {
                    if (waiting.status == Status.WAIT || waiting.status == Status.CALL) {
                        target = waiting;
                        break;
                    }
                }

                if (target == null) {
                    JOptionPane.showMessageDialog(this, "취소할 대기 내역이 없습니다.");
                    return;
                }

                int answer = JOptionPane.showConfirmDialog(
                        this,
                        String.format("%03d번 대기를 취소하시겠습니까?", target.waitingNum),
                        "대기 취소 확인",
                        JOptionPane.YES_NO_OPTION
                );

                if (answer != JOptionPane.YES_OPTION) {
                    return;
                }

                WaitingService.OperationResult result =
                        waitingService.cancelMyWaiting(loginMember.memberId, target.waitingNum);

                JOptionPane.showMessageDialog(this, result.getMessage());

                if (result.isSuccess()) {
                    showCheckOrderPanel();
                }

            } catch (SQLException ex) {
                JOptionPane.showMessageDialog(this, "대기 취소 중 오류가 발생했습니다.\n" + ex.getMessage());
            }
        });

        backButton.addActionListener(e -> showUserMainPanel());

        centerPanel.add(Box.createVerticalStrut(80));
        centerPanel.add(guideLabel);
        centerPanel.add(Box.createVerticalStrut(20));
        centerPanel.add(orderLabel);
        centerPanel.add(Box.createVerticalStrut(20));
        centerPanel.add(endLabel);

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(centerPanel, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);  // ← 핵심 변경

        setContentPane(panel);
        revalidate();
        repaint();
    }
    private void showWaitingRegisterPanel() {

        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(60, 60, 60, 60));

        JLabel titleLabel = new JLabel("대기 등록");
        titleLabel.setFont(new Font("맑은 고딕", Font.BOLD, 32));
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.Y_AXIS));

        JLabel countLabel = new JLabel("총 인원수를 입력해주세요");
        countLabel.setFont(new Font("맑은 고딕", Font.BOLD, 18));
        countLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JTextField countField = new JTextField();
        countField.setMaximumSize(new Dimension(Integer.MAX_VALUE, 45));
        countField.setFont(new Font("맑은 고딕", Font.PLAIN, 18));

        JButton registerButton = new JButton("등록하기");
        registerButton.setFont(new Font("맑은 고딕", Font.BOLD, 18));
        registerButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 50));

        JButton backButton = new JButton("뒤로가기");
        backButton.setFont(new Font("맑은 고딕", Font.BOLD, 18));
        backButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 50));

        backButton.addActionListener(e -> showUserMainPanel());

        registerButton.addActionListener(e -> {
            String countText = countField.getText().trim();

            if (countText.isEmpty()) {
                JOptionPane.showMessageDialog(this, "총 인원수를 입력해주세요.");
                return;
            }

            int totalCount;

            try {
                totalCount = Integer.parseInt(countText);
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(this, "숫자만 입력하세요.");
                return;
            }

            if (loginMember == null) {
                JOptionPane.showMessageDialog(this, "로그인 정보가 없습니다. 다시 로그인해주세요.");
                showLoginPanel();
                return;
            }

            WaitingService.OperationResult result =
                    waitingService.registerWaiting(
                            loginMember.memberId,
                            loginMember.name,
                            loginMember.phone,
                            totalCount
                    );

            JOptionPane.showMessageDialog(this, result.getMessage());

            if (result.isSuccess()) {
                showUserMainPanel();
            }
        });

        centerPanel.add(Box.createVerticalStrut(50));
        centerPanel.add(countLabel);
        centerPanel.add(Box.createVerticalStrut(20));
        centerPanel.add(countField);
        centerPanel.add(Box.createVerticalStrut(30));

        registerButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        backButton.setAlignmentX(Component.CENTER_ALIGNMENT);

        registerButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 50));
        backButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 50));

        centerPanel.add(registerButton);
        centerPanel.add(Box.createVerticalStrut(15));
        centerPanel.add(backButton);

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(centerPanel, BorderLayout.CENTER);

        setContentPane(panel);
        revalidate();
        repaint();
    }

    private JPanel createSignUpPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(55, 60, 55, 60));

        JLabel titleLabel = new JLabel("회원가입");
        titleLabel.setFont(new Font("맑은 고딕", Font.BOLD, 36));
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.Y_AXIS));

        JLabel nameLabel = new JLabel("이름");
        nameLabel.setFont(new Font("맑은 고딕", Font.BOLD, 16));

        signUpNameField = new JTextField();
        signUpNameField.setFont(new Font("맑은 고딕", Font.PLAIN, 18));
        signUpNameField.setMaximumSize(new Dimension(Integer.MAX_VALUE, 43));

        JLabel idLabel = new JLabel("아이디");
        idLabel.setFont(new Font("맑은 고딕", Font.BOLD, 16));

        signUpIdField = new JTextField();
        signUpIdField.setFont(new Font("맑은 고딕", Font.PLAIN, 18));
        signUpIdField.setMaximumSize(new Dimension(Integer.MAX_VALUE, 43));

        JLabel passwordLabel = new JLabel("비밀번호");
        passwordLabel.setFont(new Font("맑은 고딕", Font.BOLD, 16));

        signUpPasswordField = new JPasswordField();
        signUpPasswordField.setFont(new Font("맑은 고딕", Font.PLAIN, 18));
        signUpPasswordField.setMaximumSize(new Dimension(Integer.MAX_VALUE, 43));

        JLabel phoneLabel = new JLabel("연락처");
        phoneLabel.setFont(new Font("맑은 고딕", Font.BOLD, 16));

        signUpPhoneField = new JTextField();
        signUpPhoneField.setFont(new Font("맑은 고딕", Font.PLAIN, 18));
        signUpPhoneField.setMaximumSize(new Dimension(Integer.MAX_VALUE, 43));

        joinButton = new JButton("가입하기");
        joinButton.setFont(new Font("맑은 고딕", Font.BOLD, 17));
        joinButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 48));

        joinButton.addActionListener(e -> {
            String name = signUpNameField.getText().trim();
            String loginId = signUpIdField.getText().trim();
            String password = new String(signUpPasswordField.getPassword()).trim();
            String phone = signUpPhoneField.getText().trim();

            MemberService.MemberResult result =
                    memberService.signUp(name, phone, loginId, password, false);

            JOptionPane.showMessageDialog(this, result.getMessage());

            if (result.isSuccess()) {
                showLoginPanel();
            }
        });

        backButton = new JButton("뒤로가기");
        backButton.setFont(new Font("맑은 고딕", Font.BOLD, 17));
        backButton.setMaximumSize(new Dimension(Integer.MAX_VALUE, 48));

        backButton.addActionListener(e -> showLoginPanel());

        centerPanel.add(Box.createVerticalStrut(35));
        centerPanel.add(nameLabel);
        centerPanel.add(Box.createVerticalStrut(8));
        centerPanel.add(signUpNameField);
        centerPanel.add(Box.createVerticalStrut(20));
        centerPanel.add(idLabel);
        centerPanel.add(Box.createVerticalStrut(8));
        centerPanel.add(signUpIdField);
        centerPanel.add(Box.createVerticalStrut(20));
        centerPanel.add(passwordLabel);
        centerPanel.add(Box.createVerticalStrut(8));
        centerPanel.add(signUpPasswordField);
        centerPanel.add(Box.createVerticalStrut(20));
        centerPanel.add(phoneLabel);
        centerPanel.add(Box.createVerticalStrut(8));
        centerPanel.add(signUpPhoneField);
        centerPanel.add(Box.createVerticalStrut(32));
        centerPanel.add(joinButton);
        centerPanel.add(Box.createVerticalStrut(15));
        centerPanel.add(backButton);

        panel.add(titleLabel, BorderLayout.NORTH);
        panel.add(centerPanel, BorderLayout.CENTER);

        return panel;
    }

    public static void main(String[] args) {
        new MainSwing();
    }
}