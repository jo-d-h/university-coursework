<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>밥도둑 데모</title>
    <link rel="stylesheet" href="mainCss.css">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Jua&display=swap" rel="stylesheet">
</head>
<body>
    <table class="header-table">
        <tr>
            <td class="header-logoBox">
                <img src="deulogo.png" alt="동의대로고" class="header-logo">
            </td>

            <td class="header-title" onclick="location.href='index.php'">
                <p>밥도둑</p>
            </td>

            <td class="user-menu" id="user-menu">
                 <img src="사용자.png" alt="사용자" class="user-logo"> 
            </td>
        </tr>
    </table>

    <div class="search-section">
        <div class="search-info">
            <p>교내 맛집을 찾아드리는 밥도둑입니다!</p>
        </div>
        
        <div class="search-bar-wrapper">
            <span class="search-icon" id="search-button">🔍</span>
            <input type="text" class="search-input" id="search-input" placeholder="메뉴로 검색 식당명으로 검색">
        </div>
    </div>

    <p class="question">어디서 드시나요?</p>
    <table class="category-table">
        <tr>
            <td><div class="category-button" onclick="location.href='getShops.html?location=수덕전'">
                <img src="수덕전.jpg" alt="수덕전 이미지" class="category-image">
            </div><div class="category-label">수덕전</div></td>

            <td><div class="category-button" onclick="location.href='getShops.html?location=지천관'">
                <img src="지천관.jpg" alt="지천관 이미지" class="category-image">
            </div><div class="category-label">지천관</div></td>

            <td><div class="category-button" onclick="location.href='getShops.html?location=행복기숙사'">
                <img src="행긱.jpg" alt="행긱 이미지" class="category-image">
            </div><div class="category-label">행복 기숙사</div></td>

            <td><div class="category-button" onclick="location.href='getShops.html?location=국제관'">
                <img src="국제관.jpg" alt="국제관 이미지" class="category-image">
            </div><div class="category-label">국제관</div></td>

            <td><div class="category-button" onclick="window.location.href='getShops.html?location=기타'">
                ...</div><div class="category-label">기타</div></td>
        </tr>
    </table>

    <p class="question">무엇을 드시나요?</p>
    <table class="category-table">
        <tr>
            <td><div class="category-button" onclick="window.location.href='getShops.html?keyword=컵밥'">
                <img src="컵밥.png" alt="컵밥" class="category-image">
            </div><div class="category-label">컵밥</div></td>

            <td><div class="category-button" onclick="window.location.href='getShops.html?keyword=햄버거,샌드위치'">
                <img src="햄버거.png" alt="햄버거" class="category-image">
            </div><div class="category-label">햄버거 샌드위치</div></td>

            <td><div class="category-button" onclick="window.location.href='getShops.html?keyword=커피'">
                <img src="커피.png" alt="커피" class="category-image">
            </div><div class="category-label">커피</div></td>

            <td><div class="category-button" onclick="window.location.href='getShops.html?keyword=피자'">
                <img src="피자.png" alt="피자" class="category-image">
            </div><div class="category-label">피자</div></td>

            <td><div class="category-button" onclick="window.location.href='getShops.html?keyword=분식'">
                <img src="간식.png" alt="간식" class="category-image">
            </div><div class="category-label">간식</div></td>
        </tr>
    </table>

    <div class="recommend-header">
        <div>
            <span id="best-tab" class="ranking-active">맛집랭킹</span>
            <span id="solo-tab" class="ranking-inactive">혼밥랭킹</span>
        </div>
        <span class="more-button" onclick="location.href='ranking.html'">더보기</span>
    </div>

    <table class="item-list-table">
        <tr id="ranking-row"></tr>
    </table>

    <div class="copyright-section" onclick="location.href='db_install.php'">
        <p>팀원: 20223123 이승환, 20213025 이현승, 20223156 조동현</p>
        <p>프로젝트: 밥도둑</p>
    </div>

    <script>
        const searchInput = document.getElementById('search-input');
        const searchButton = document.getElementById('search-button');

        function searchShops() {
            const query = searchInput.value.trim();
            if (!query) return;
            window.location.href = `getShops.html?search=${encodeURIComponent(query)}`;
        }

        searchButton.addEventListener('click', searchShops);
        searchInput.addEventListener('keydown', e => {
            if (e.key === 'Enter') searchShops();
        });

        // 랭킹 데이터 불러오기 및 출력
        let rankingData = {};

        fetch('getRanking.php')
            .then(res => res.json())
            .then(data => {
                rankingData = data;
                showRanking('best'); // 기본은 맛집랭킹
            });

        function showRanking(type) {
            const row = document.getElementById('ranking-row');
            row.innerHTML = '';

            rankingData[type].slice(0, 4).forEach((shop, index) => {
                const td = document.createElement('td');
                td.innerHTML = `
                    <div class="item-box">
                        <a href="shop_detail.php?id=${shop.id}" style="text-decoration: none; color: inherit;">
                        <div class="item-image">
                            <img src="${encodeURI(shop.shop_extphoto)}"  
                                alt="${shop.shop_name}"  
                                class="ranking-image"
                                onerror="if (!this.dataset.fallback) { this.src='default.jpg'; this.dataset.fallback = true; }">
                        </div>
                        <div class="item-info">
                            <p class="item-name">${index + 1}. ${shop.shop_name}</p>
                            <p>📍 ${shop.shop_loca}</p>
                            <p class="item-tags">#${shop.shop_menu}</p>
                        </div>
                        </a>
                    </div>
                `;
                row.appendChild(td);
            });

            // 탭 스타일 전환
            document.querySelector('.ranking-active').classList.remove('ranking-active');
            document.querySelector('.ranking-inactive').classList.remove('ranking-inactive');
            const tabs = document.querySelectorAll('.recommend-header span');
            tabs.forEach(tab => {
                if (tab.textContent.includes(type === 'best' ? '맛집' : '혼밥')) {
                    tab.classList.add('ranking-active');
                } else {
                    tab.classList.add('ranking-inactive');
                }
            });
        }

        document.getElementById('best-tab').addEventListener('click', () => showRanking('best'));
        document.getElementById('solo-tab').addEventListener('click', () => showRanking('solo'));

        // user-menu 클릭 시 로그인 여부 확인
        document.getElementById('user-menu').addEventListener('click', () => {
            fetch('checkLogin.php')
                .then(res => res.json())
                .then(data => {
                    if (data.loggedIn) {
                        window.location.href = 'mypage.php';
                    } else {
                        window.location.href = 'login.php';
                    }
                })
                .catch(err => {
                    console.error('로그인 상태 확인 실패:', err);
                    window.location.href = 'login.php'; // 오류 시 기본적으로 로그인 페이지로
                });
        });
    </script>
</body>
</html>