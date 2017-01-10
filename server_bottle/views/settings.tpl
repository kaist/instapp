
<!DOCTYPE html>
  <html>
    <head>
      <!--Import Google Icon Font-->
      <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
      <!--Import materialize.css-->
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.8/css/materialize.min.css">

      <!--Let browser know website is optimized for mobile-->
      <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    </head>

    <body>
      <div class="container">
<div class="center-align">
<h2>instAPP</h2>
</div>



<div id="logintoinsta" style="display:none;">
      <div class="row">
        <div class="col s12 m7">
          <div class="card">

            <div class="card-content">

<h5 class=" center-align" ><span>Login to instagram</span></h5>

  <div class="row">
    <form class="col s12">
      <div class="row">
        <div class="input-field col s12">
          <input  id="loginf" type="text">
          <label for="login">Login</label>
        </div>

      </div>

      <div class="row">
        <div class="input-field col s12">
          <input  id="passw" type="password" type="text">
          <label for="passw">Password</label>
        </div>

      </div>
<div id="incorrect" class=" center-align" style="display:none;color:#b71c1c;"><span>Login or password incorrect</span></div>
      <div class="row">
        <div class="center-align input-field col s12">
<a id="loginbutton" class="waves-effect waves-light btn"><i class="material-icons left">done</i>Login</a>
        </div>

      </div>

      <div class="row">
        <div class="center-align input-field col s12">
  <div id="dload" style="display:none;" class="preloader-wrapper active">
    <div class="spinner-layer spinner-red-only">
      <div class="circle-clipper left">
        <div class="circle"></div>
      </div><div class="gap-patch">
        <div class="circle"></div>
      </div><div class="circle-clipper right">
        <div class="circle"></div>
      </div>
    </div>
  </div>
        </div>

      </div>


    </form>
  </div>
            </div>

          </div>
        </div>
      </div>
</div>


<div id="start">
      <div class="row">
        <div class="col s12 m7">
          <div class="card">

            <div class="card-content">

<h5 class=" center-align" style="color:#b71c1c;"><span>Security warning!</span></h5>
Username and password are sent to the third-party site. If you do not trust the author of the program, give up the use of, or use your own server. The source code of the application and the server is available on githab <a href="https://github.com/kaist/instapp">https://github.com/kaist/instapp</a>
<p style="margin-top: 20px;"></p>
<div class="center-align">
<p>
<a id="loginform" class="waves-effect waves-light btn">I trust. Use instapp.tk server</a>
</p>

<p style="margin-top: 20px;">
<a  id="newserver"  href="#modal1" class="waves-effect waves-light red btn">Use external server</a>
</p>
</div>
            </div>

          </div>
        </div>
      </div>
</div>

<div id="settings" style="display:none;">
     <div class="col s12 m12 ">
        <div class="card-panel grey lighten-5 z-depth-1">
          <div class="row valign-wrapper">

            <div class="col s12">
    <p><b>Number of photos for download</b></p>
    <p class="range-field">
      <input type="range" id="ttt" min="3" max="20" />
    </p>
    <blockquote>
А large amount increases the run time application
    </blockquote>



            </div>



          </div>

<div class="row right-align">

            <div class=" right-align col s12">
            <a id="saveall" class="waves-effect waves-light btn"><i class="material-icons left">save</i>Save</a>
            </div>
      </div>
        </div>
      </div>
<div class="col s12 center-align">
<p><a onclick="deleteall();" href="#">Logout and clear all data</a></p>
<p style="margin-top: 80px;">

<form action="https://www.paypal.com/cgi-bin/webscr" method="post" target="_top">
<input type="hidden" name="cmd" value="_s-xclick">
<input type="hidden" name="hosted_button_id" value="YXTU2JSPNZG56">
<input type="image" src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif" border="0" name="submit" alt="PayPal - The safer, easier way to pay online!">
<img alt="" border="0" src="https://www.paypalobjects.com/ru_RU/i/scr/pixel.gif" width="1" height="1">
</form>
</p>
<p>
<iframe frameborder="0" allowtransparency="true" scrolling="no" src="https://money.yandex.ru/quickpay/button-widget?account=4100187212636&quickpay=small&any-card-payment-type=on&button-text=04&button-size=m&button-color=orange&targets=%D0%9D%D0%B0+%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D1%83+InstAPP&default-sum=50&successURL=" width="223" height="36"></iframe>
</p>

</div>
</div>


  <!-- Modal Trigger -->

  <!-- Modal Structure -->
  <div id="modal1" class="modal">
    <div class="modal-content">
      <p>Server adress</p>


      <div class="row">
        <div class="input-field col s12">
          <input value="https://" id="servername" type="text">

        </div>

      </div>



    </div>
    <div class="modal-footer">
          <a href="#!" class=" modal-action modal-close waves-effect waves-green btn-flat">Close</a>
      <a href="#!" onclick="save_external();" class=" waves-effect waves-green btn-flat">Save</a>

    </div>
  </div>

      </div>


      <script type="text/javascript" src="https://code.jquery.com/jquery-2.1.1.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.8/js/materialize.min.js"></script>
<script>
$('#modal1').modal();
var varn=0;
var url="http://instapp.tk";
var token='';
var new_width={{sets}};
var new_serv='';
var is_delete=0;




%if is_new:
  $('#settings').show();
  $('#start').hide();
%end

$('#ttt').val(new_width);


$('#saveall').click(

function ()
{


	new_width=$('#ttt').val();
  send();

}



	);


$('#loginform').click(
      function()
      {
        varn=1;
        $('#start').hide();
        $('#logintoinsta').show();
      }

  );

$('#loginbutton').click(
      function()
      {
        l=$('#loginf').val();
        p=$('#passw').val();
        $('#loginbutton').addClass('disabled')
        $('#dload').show();
        $('#incorrect').hide();

        $.ajax({
  			type: "POST",
 			url: url+'/login',
  			data: {login:l,passw:p},
  			success: function (resp){
  				d=JSON.parse(resp)
  				if (!d.login)
  					{ $('#loginbutton').removeClass('disabled');
  						$('#incorrect').show();
  						$('#dload').hide();

  					}
  				else {
  					token=d.token;
  					$('#logintoinsta').hide();
  					$('#settings').show();




  				}

  			}
		});

     	 }

  );

function deleteall(){
	is_delete=1;
  send();
}

function send(){

   var options = {
      'is_delete': is_delete,
      'token': token,
      'sets': new_width,
      'server': new_serv
    };

    // Determine the correct return URL (emulator vs real watch)
    function getQueryParam(variable, defaultValue) {
      var query = location.search.substring(1);
      var vars = query.split('&');
      for (var i = 0; i < vars.length; i++) {
        var pair = vars[i].split('=');
        if (pair[0] === variable) {
          return decodeURIComponent(pair[1]);
        }
      }
      return defaultValue || false;
    }
    var return_to = getQueryParam('return_to', 'pebblejs://close#');

    // Encode and send the data when the page closes
    document.location = return_to + encodeURIComponent(JSON.stringify(options));
};

function save_external(){
	serv=$('#servername').val();
	url=serv;
	new_serv=serv;
	$('#modal1').modal('close');
       varn=1;
       $('#start').hide();
       $('#logintoinsta').show();
}
</script>
    </body>
  </html>