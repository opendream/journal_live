<div class="account-profile block-bg-shadow">
	<?php print $account->user_info['display_picture'];?>
	<div class="account-profile-content">
		
    <div class="account-profile-item">
			<div class="account-profile-label label">Username: </div><div><?php print $account->name;?></div>
		</div>
    
    <?php if(isset($account->data['firstname'])):?>
		<div class="account-profile-item">
			<div class="account-profile-label label">First name: </div><div><?php print $account->data['firstname'];?></div>
		</div>
    <?php endif;?>

    <?php if(isset($account->data['lastname'])):?>
		<div class="account-profile-item">
			<div class="account-profile-label label">Last name: </div><div><?php print $account->data['lastname'];?></div>
		</div>
    <?php endif;?>
		
    <div class="account-profile-item">
			<div class="account-profile-label label">E-mail address: </div><div><?php print $account->mail;?></div>
		</div>

	</div>
</div>
