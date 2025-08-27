document.getElementById('nav').onmouseover = function(event){
    //отслеживает нахождение мыши внутри блока
    var target = event.target;
    //отслеживаем клик
    if (target.className == 'menu-item'){
        var s = target.getElementsByClassName('submenu');
        closeMenu();
        s[0].style.display='block';
        //массив, содержащий ложное меню
    }
}
document.onmouseover = function(event){
    var target = event.target;
    console.log(event.target);
    if (target.className!='menu-item' && target.className!='submenu'){
        closeMenu();
    }
}
    function closeMenu(){
    //получение всех элементов подменю в блоке nav, присваивание display:none
    var menu = document.getElementById('nav');
    var subm = document.getElementsByClassName('submenu');
    for (var i=0; i <subm.length; i++) {
        subm[i].style.display ="none";
    }
}