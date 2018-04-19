jQuery(document).ready(function() {
    var refreshStatus = function() {
        $('#status').text('Loading status...').show();
        $.ajax({
            url: 'status'
        }).done(function(data) {
            console.log(data);
            if (data == 'on') {
                $('#linkOn').removeClass('hidden');
                $('#linkOff').addClass('hidden');
            } else if (data == 'off') {
                $('#linkOn').addClass('hidden');
                $('#linkOff').removeClass('hidden');
            }
            $('#status').hide();
        });
    };
    $('#linkOn').click(function(e) {
        e.preventDefault();
        $('#status').text('Sending On-command...').show();
        $('#linkOn').addClass('hidden');
        $.ajax({
            url: $('#linkOn').attr('href')
        }).done(function(data) {
            console.log(data);
            if (data != 'success') {
                alert('error');
            }
            refreshStatus();
        });
    });
    $('#linkOff').click(function(e) {
        e.preventDefault();
        $('#status').text('Sending Off-command...').show();
        $('#linkOff').addClass('hidden');
        $.ajax({
            url: $('#linkOff').attr('href')
        }).done(function(data) {
            console.log(data);
            if (data != 'success') {
                alert('error');
            }
            refreshStatus();
        });
    });
    setInterval(refreshStatus, 15000);
    refreshStatus();
});
