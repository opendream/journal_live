jQuery(document).ready(function(){

  var $ = jQuery;
  
  $('.load-more-ajax').click(function (e){
    
    e.preventDefault();
    var container_list = $('#' + $(this).attr('rel'));
    var load_more_link = $(this);

    $.getJSON($(this).attr('href'), {'ajax': 1}, function (resp) {

      $.each(resp.item_list, function (i, item) {
        container_list.append($(item));
      });

      if(resp.update_load_more_url) {
        load_more_link.attr('href', resp.update_load_more_url);
      }
      else {
        load_more_link.remove();
      }

    });

  });
  
});

