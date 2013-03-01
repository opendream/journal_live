jQuery(document).ready(function () {
  $ = jQuery;
  
  var hrlp_qtip_options = function() {}
  
  $('.label-help').each(function () {
    
    var helptext = $(this).html();
    var corner = {
       tooltip: 'bottomRight', // Use the corner...
       target: 'topLeft' // ...and opposite corner
    };
    if ($(this).hasClass('tip-right')) {
      corner = {
         tooltip: 'bottomLeft', // Use the corner...
         target: 'topRight' // ...and opposite corner
      };
    };
    
    
    var options = {
       content: helptext,
       position: {
          corner: corner
       },
       show: {
         solo: true,
         when: {
           'event': 'focus'
         }
       },
       hide: {
         when: {
            'event': 'blur'
          }
       },
       style: {
          border: {
             width: 3,
             radius: 7
          },
          padding: 10, 
          textAlign: 'left',
          tip: true, // Give it a speech bubble tip with automatic corner detection
          name: 'blue' // Style it according to the preset 'cream' style
       }
       
    }
    
    // When mouseover and input focus show tool tip ja
    var label = $(this).parent('label');
    if (label.length) {
      $(this).qtip(options);
      options.show.when.target = $('#' + label.attr('for'));
      options.hide.when.target = $('#' + label.attr('for'));
      $(this).qtip(options);
    }
    
    var legend = $(this).parent('.fieldset-legend').parent('legend');
    var fieldset = legend.parent('.form-wrapper');
    if (fieldset.length) {
      
      legend.click(function () {
        fieldset.find('input.multi-textfield').eq(0).focus();
      });
      
      options.show.when.target = fieldset.find('input.multi-textfield');
      options.hide.when.target = fieldset.find('input.multi-textfield');
      $(this).qtip(options);
    }

    options.show = 'mouseover';
    options.hide = 'mouseout';
    $(this).qtip(options);

    
  });

})