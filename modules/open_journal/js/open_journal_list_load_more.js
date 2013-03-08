jQuery(document).ready(function(){

  var $ = jQuery;
  
  $('#journal-list-load-more').click(function (e){

    $.getJSON($(this).attr('href'), function (resp) {

      console.log(resp);

    });

  });
  
});

