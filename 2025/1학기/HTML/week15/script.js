// 제목 및 설명
const h1 = document.createElement("h1");
h1.textContent = "자연연역 연습장";
document.body.appendChild(h1);

const p = document.createElement("p");
p.textContent = "자연연역을 연습할 수 있는 공간입니다. 아래에 문제를 입력하고 복합명제를 추가하세요.";
document.body.appendChild(p);

// 문제 입력창
const textarea = document.createElement("textarea");
textarea.style.width = "800px";
textarea.style.height = "75px";
textarea.value = `아담이 백만장자라면, 가난은 이브가 그의 청혼을 거절한 이유가 아니다.
그런데 이브가 그의 청혼을 거절한 이유는 아담이 가난하다는 것 또는 그가 매력이 없다는 것 둘 중의 하나이다.
아담은 백만장자이다. 그러므로 매력이 없다는 것이 아담이 청혼을 거절당한 이유임에 틀림없다.`;
document.body.appendChild(textarea);

// 리스트 스타일
function styleListBox(list) {
    list.style.border = "1px solid #ccc";
    list.style.padding = "10px";
    list.style.borderRadius = "6px";
    list.style.marginTop = "10px";
    list.style.width = "785px";
    list.style.fontSize = "14px";
    list.style.fontFamily = "sans-serif";
    list.style.listStyle = "none";
}

// 항목 생성
function createStyledItem(prefix, text) {
    const li = document.createElement("li");
    li.style.marginBottom = "5px";
    li.style.lineHeight = "1.6";

    const span = document.createElement("span");
    span.textContent = `${prefix} ${text} `;

    const btn = document.createElement("button");
    btn.textContent = "[X]";
    btn.style.border = "none";
    btn.style.background = "none";
    btn.style.color = "red";
    btn.style.cursor = "pointer";
    btn.style.fontSize = "13px";
    btn.style.padding = "0";
    btn.style.marginLeft = "5px";
    btn.addEventListener("click", () => li.remove());

    li.appendChild(span);
    li.appendChild(btn);
    return li;
}

// 복합명제 입력
const compoundInput = document.createElement("input");
compoundInput.placeholder = "복합명제를 입력하세요.";
compoundInput.style.width = "800px";
compoundInput.style.height = "25px";
compoundInput.style.marginRight = "10px";

const compoundBtn = document.createElement("button");
compoundBtn.textContent = "추가";
compoundBtn.type = "button"; // ✅ 추가
compoundBtn.style.height = "25px";

const compoundForm = document.createElement("form");
compoundForm.appendChild(compoundInput);
compoundForm.appendChild(compoundBtn);
document.body.appendChild(compoundForm);

const compoundList = document.createElement("ul");
styleListBox(compoundList);
document.body.appendChild(compoundList);

// 단순명제 입력
const primaryInput = document.createElement("input");
primaryInput.placeholder = "단순명제를 입력하세요.";
primaryInput.style.width = "800px";
primaryInput.style.height = "25px";
primaryInput.style.marginRight = "10px";

const primaryBtn = document.createElement("button");
primaryBtn.textContent = "추가";
primaryBtn.type = "button"; // ✅ 추가
primaryBtn.style.height = "25px";

const primaryForm = document.createElement("form");
primaryForm.appendChild(primaryInput);
primaryForm.appendChild(primaryBtn);
document.body.appendChild(primaryForm);

const primaryList = document.createElement("ul");
styleListBox(primaryList);
document.body.appendChild(primaryList);

// 번호 카운터
let compoundCount = 1;
let primaryCount = 0;
let symbolicCount = 1;

// 복합명제 추가 이벤트
compoundBtn.addEventListener("click", () => {
    const value = compoundInput.value.trim();
    if (!value) return;
    const li = createStyledItem(`${compoundCount}.`, value);
    compoundList.appendChild(li);
    compoundInput.value = "";
    compoundInput.focus();
    compoundCount++;
});

// 단순명제 추가 이벤트
primaryBtn.addEventListener("click", () => {
    const value = primaryInput.value.trim();
    if (!value) return;
    const letter = String.fromCharCode(65 + primaryCount); // A, B, ...
    const li = createStyledItem(`${letter}.`, value);
    primaryList.appendChild(li);
    primaryInput.value = "";
    primaryInput.focus();
    primaryCount++;
});

// 기호명제 입력
const symbolicForm = document.createElement("form");
symbolicForm.style.marginTop = "10px";

const symbolicInput = document.createElement("input");
symbolicInput.placeholder = "기호명제를 입력하세요.";
symbolicInput.style.width = "462px";
symbolicInput.style.height = "25px";
symbolicInput.style.marginRight = "10px";

// ns1~3
const ns1 = document.createElement("input");
ns1.placeholder = "1";
ns1.style.width = "40px";
ns1.style.height = "25px";
ns1.style.marginRight = "5px";

const ns2 = document.createElement("input");
ns2.placeholder = "2";
ns2.style.width = "40px";
ns2.style.height = "25px";
ns2.style.marginRight = "5px";

const ns3 = document.createElement("input");
ns3.placeholder = "3";
ns3.style.width = "40px";
ns3.style.height = "25px";
ns3.style.marginRight = "5px";

// 규칙 선택
const ruleSelect = document.createElement("select");
ruleSelect.innerHTML = `
  <option value="전제">전제</option>
  <option value="결론">결론</option>
  <option value="→ 제거">조건기호제거 (→제거)</option>
  <option value="∨ 제거">선언기호제거 (∨제거)</option>
  <option value="∨ 도입">선언기호도입 (∨도입)</option>
  <option value="→ 도입">조건기호도입 (→도입)</option>
  <option value="∧ 제거">연언기호제거 (∧제거)</option>
  <option value="∧ 도입">연언기호도입 (∧도입)</option>
  <option value="↔ 도입">쌍조건기호도입 (↔도입)</option>
  <option value="↔ 제거">쌍조건기호제거 (↔제거)</option>
  <option value="∼ 제거">부정기호제거 (∼제거)</option>
  <option value="∼ 도입">부정기호도입 (∼도입)</option>
`;
ruleSelect.style.height = "30px";
ruleSelect.style.marginLeft = "10px";

// 기호명제 추가 버튼
const symbolicBtn = document.createElement("button");
symbolicBtn.textContent = "추가";
symbolicBtn.type = "button"; // ✅ 유지
symbolicBtn.style.height = "25px";
symbolicBtn.style.marginLeft = "10px";

// 조립
symbolicForm.appendChild(symbolicInput);
symbolicForm.appendChild(ns1);
symbolicForm.appendChild(ns2);
symbolicForm.appendChild(ns3);
symbolicForm.appendChild(ruleSelect);
symbolicForm.appendChild(symbolicBtn);
document.body.appendChild(symbolicForm);

// 출력 리스트
const symbolicList = document.createElement("ul");
styleListBox(symbolicList);
document.body.appendChild(symbolicList);

// 기호명제 추가 이벤트
symbolicBtn.addEventListener("click", () => {
    const sym = symbolicInput.value.trim();
    const rule = ruleSelect.value;
    const refs = [ns1.value.trim(), ns2.value.trim(), ns3.value.trim()].filter(Boolean).join(", ");

    if (!sym) return;

    const refText = refs ? ` (${refs})` : "";
    const li = createStyledItem(`${symbolicCount}.`, `${sym} [${rule}]${refText}`);
    symbolicList.appendChild(li);   

    symbolicInput.value = "";
    ns1.value = "";
    ns2.value = "";
    ns3.value = "";
    symbolicInput.focus();
    symbolicCount++;
});

const info = document.createElement("p");
info.textContent = "20223156 조동현";

const link = document.createElement("a");
link.href = "https://github.com/moonmoon4123/week15";
link.textContent = " GitHub Repository";
link.target = "_blank";  // 새 창에서 열기
//link.style.marginLeft = "10px";
link.style.textDecoration = "underline";
link.style.color = "blue";

info.appendChild(link);
document.body.appendChild(info);