function onEnter(){
window.location.href = "layout/pg.html";

}
const hamburgerBtn = document.getElementById("hamburgerBtn");
const sidebar = document.getElementById("sidebar");

hamburgerBtn.addEventListener("click", () => {
  sidebar.classList.toggle("active");
  hamburgerBtn.classList.toggle("active");
});
