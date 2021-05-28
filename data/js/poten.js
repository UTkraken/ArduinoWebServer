   $(".ValueRed").html($("#Red").val());
   $(".ValueGreen").html($("#Green").val());
   $(".ValueBlue").html($("#Blue").val());


$("[type=range]").on("change ", function() {
   var red = $(".ValueRed").html($("#Red").val());
   var green = $(".ValueGreen").html($("#Green").val());
   var blue = $(".ValueBlue").html($("#Blue").val());

   sendColor(red,green,blue);


});


function sendColor(red, green, blue) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var ok = this.responseText;
      console.log(ok);
    }
  };
  xhttp.open("GET", "led?red="+red+"&green="+green+"&blue="+blue, true);
  xhttp.send();
}