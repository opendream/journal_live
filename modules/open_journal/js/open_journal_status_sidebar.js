jQuery(document).ready(function () {

  $ = jQuery;


  function bind_btn_action(code) {
    
    $('#open-journal-'+code+'-sidebar-form').hide();
    $('#'+code+'_bottom').click(function(e){
      e.preventDefault();
      
    });
  

    $('#'+code+'_bottom').qtip({
        content: $('#open-journal-'+code+'-sidebar-form'),
        show: {
        	when: {
        		'event': 'click'
        	}
        },
        hide: {
        	when: {
        		target: $('#'+code+'-cancel-link'),
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

    $('#'+code+'-cancel-link').click(function (e) {
    	e.preventDefault();

    	$('#'+code+'_bottom').qtip('hide');

    })
  }

  bind_btn_action('reject');
  bind_btn_action('withdraw');

});