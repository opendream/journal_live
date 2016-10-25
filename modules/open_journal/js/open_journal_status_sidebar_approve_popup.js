jQuery(document).ready(function () {

  $ = jQuery;
  // Approve popup btn
  $('#open-journal-approve-sidebar-form').hide();
  $('#approve_popup_bottom').click(function(e){
    e.preventDefault();
    
  });
  

  $('#approve_popup_bottom').qtip({
      content: $('#open-journal-approve-sidebar-form'),
      show: {
        when: {
          'event': 'click'
        }
      },
      hide: {
        when: {
          target: $('#approve-popup-cancel-link'),
          'event': 'click'
        }

      },
      position: {
          corner: {
            tooltip: 'rightBottom', // Use the corner...
          target: 'topLeft' // ...and opposite corner
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
   });

  $('#approve-popup-cancel-link').click(function (e) {
    e.preventDefault();

    $('#approve_popup_bottom').qtip('hide');

  })

});