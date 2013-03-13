<div class="account-profile block-bg-shadow">
	<div><?php print $account->user_info['display_picture'];?></div>
	<div>Username: </div><div><?php print $account->name;?></div>
	<div>First name: </div>
	<div><?php if(isset($account->data['firstname'])):?><?php print $account->data['firstname'];?><?php endif;?></div>

	<div>Last name: </div>
	<div><?php if(isset($account->data['lastname'])):?><?php print $account->data['lastname'];?><?php endif;?></div>

	<div>E-mail address: </div><div><?php print $account->mail;?></div>
</div>