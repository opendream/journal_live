jQuery(document).ready(function () {

  $ = jQuery;
  $('#open-journal-reject-sidebar-form').hide();
  $('#reject_bottom').click(function(e){
  	e.preventDefault();
  	
  });
  

  $('#reject_bottom').qtip({
      content: $('#open-journal-reject-sidebar-form'),
      show: {
      	when: {
      		'event': 'click'
      	}
      },
      hide: {
      	when: {
      		target: $('#reject-cancel-link'),
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

  $('#reject-cancel-link').click(function (e) {
  	e.preventDefault();

  	$('#reject_bottom').qtip('hide');

  })
});