<?php
    echo "<h2>설문조사 결과</h2>";

    // 텍스트 입력 필드 정보 가져오기
    $sname = $_POST['sname'] ?? '정보 없음';
    echo "이름: " . htmlspecialchars($sname) . "<br/>";

    $snum = $_POST['snum'] ?? '정보 없음';
    echo "학번: " . htmlspecialchars($snum) . "<br/>";

    $sdept = $_POST['sdept'] ?? '정보 없음';
    echo "학과이름(전공): " . htmlspecialchars($sdept) . "<br/><br/>";

    // 라디오 버튼 선택 결과 가져오기
    $poll01_val = [
        'poll01A' => '매우 그렇다', 'poll01B' => '그렇다', 'poll01C' => '보통이다', 'poll01D' => '아니다', 'poll01E' => '매우 아니다'
    ];
    $poll01 = $_POST['poll01'] ?? '';
    echo "1. 나는 프로그래밍에 대해 자신이 있다: " . ($poll01_val[$poll01] ?? '선택 안 함') . "<br/>";

    $poll02_val = [
        'poll02A' => '매우 그렇다', 'poll02B' => '그렇다', 'poll02C' => '보통이다', 'poll02D' => '아니다', 'poll02E' => '매우 아니다'
    ];
    $poll02 = $_POST['poll02'] ?? '';
    echo "2. 게임이 만들어지는 과정에 대해 이해가 있다: " . ($poll02_val[$poll02] ?? '선택 안 함') . "<br/>";
    
    $poll03_val = [
        'poll03A' => '매우 그렇다', 'poll03B' => '그렇다', 'poll03C' => '보통이다', 'poll03D' => '아니다', 'poll03E' => '매우 아니다'
    ];
    $poll03 = $_POST['poll03'] ?? '';
    echo "3. 게임을 만들어 본 경험이 있다: " . ($poll03_val[$poll03] ?? '선택 안 함') . "<br/><br/>";

    // 체크박스 선택 결과 가져오기
    echo "1. 프로그래밍 동아리 가입 시 가장 중요하게 생각하는 부분: ";
    $poll10_options = [
        'poll10A' => '자료제공(콘텐츠)', 'poll10B' => '스터디 모임(자격증)', 'poll10C' => '커뮤니티',
        'poll10D' => '이벤트', 'poll10E' => '게임 만들기', 'poll10F' => '공모전 참여',
        'poll10G' => '기타의견'
    ];
    $selected_poll10 = [];
    foreach ($poll10_options as $key => $value) {
        if (isset($_POST[$key])) {
            $selected_poll10[] = $value;
        }
    }
    if (isset($_POST['poll10H']) && $_POST['poll10H'] !== '') {
        $selected_poll10[] = '기타의견: ' . htmlspecialchars($_POST['poll10H']);
    }
    echo implode(', ', $selected_poll10) . "<br/>";
    
    echo "2. 자신이 다룰 수 있는 프로그래밍 언어는: ";
    $poll20_options = [
        'poll20A' => 'C언어(C++,C#)', 'poll20B' => '파이썬', 'poll20C' => '자바', 'poll20D' => '기타'
    ];
    $selected_poll20 = [];
    foreach ($poll20_options as $key => $value) {
        if (isset($_POST[$key])) {
            $selected_poll20[] = $value;
        }
    }
    echo implode(', ', $selected_poll20) . "<br/>";

    echo "3. 만들고 싶은 게임/선호하는 게임의 종류: ";
    $poll30_options = [
        'poll30A' => 'RPG게임', 'poll30B' => 'AOS게임', 'poll30C' => 'FPS게임', 'poll30D' => '기타'
    ];
    $selected_poll30 = [];
    foreach ($poll30_options as $key => $value) {
        if (isset($_POST[$key])) {
            $selected_poll30[] = $value;
        }
    }
    echo implode(', ', $selected_poll30) . "<br/>";

    echo "4. 이해하고 있는/다룰 수 있는 게임 엔진: ";
    $poll40_options = [
        'poll40A' => '유니티', 'poll40B' => '언리얼', 'poll40C' => '소스엔진', 'poll40D' => '주피터', 'poll40E' => '기타'
    ];
    $selected_poll40 = [];
    foreach ($poll40_options as $key => $value) {
        if (isset($_POST[$key])) {
            $selected_poll40[] = $value;
        }
    }
    echo implode(', ', $selected_poll40) . "<br/><br/>";

    // 나머지 필드 정보 가져오기
    $poll50 = $_POST['poll50'] ?? '정보 없음';
    echo "5. 같은 팀 희망 멤버: " . htmlspecialchars($poll50) . "<br/>";

    $poll60 = $_POST['poll60'] ?? '';
    $mentor_options = [
        'poll60A' => '신재현', 'poll60B' => '구아름', 'poll60C' => '윤서진', 'poll60D' => '김준혁'
    ];
    $mentor_name = $mentor_options[$poll60] ?? '선택 안 함';
    echo "6. 희망하는 멘토: " . htmlspecialchars($mentor_name) . "<br/>";

    echo "<br/><br/><p>설문조사에 응해 주셔서 감사합니다.</p>";
?>