<div id="user-heading">
	<h1>Journals List</h1>
	<?php print $journal_list['user_info']['display_picture'];?>
	<h2><?php print $journal_list['user_info']['display_name'];?></h2>
	<span>4 Journals</span>
</div>

<div class="journal-filter-tab">
	<div class="journal-filter">
		<div class="journal-filter-active">All Journals</div>
	</div>
	<ul id="journal-filter-item-list">
		<li class="journal-filter-item select-item"><a href="#">All Journals</a></li>
		<li class="journal-filter-item"><a href="#">Option 1</a></li>
		<li class="journal-filter-item"><a href="#">Option 2</a></li>
		<li class="journal-filter-item"><a href="#">Option 3</a></li>
		<li class="journal-filter-item"><a href="#">Option 4</a></li>
	</ul>
	<div class="add-journal-btn">
		<?php print l(t('Add New'), OPEN_JOURNAL_PREFIX_PATH.'/add'); ?>
	</div>
</div>

<div id="journal-list">
	<ul>
		<?php foreach ($journal_list['list'] as $value):?>
		<li class="journal-item">
			<div class="journal-status information-approve"><span><?php print $value->status;?></span></div>
			<h3 class="journal-title"><a href="/journal/1/overview"><?php print $value->dc_title;?></a></h3>
			<div class="journal-update">
				<span class="label">Status update: </span>
				<span><?php print date('d M Y', $value->status_changed);?></span>
			</div>
			<div class="journal-create">
				<span class="label">Create Date: </span>
				<span><?php print date('d M Y', $value->created);?></span>		
			</div>
			<ul class="relate-peoples">
				<?php foreach ($value->list_people as  $value_info):?>
				<li class="clickable-people owner profile-picture-small">
				<?php $user_info_tmp = open_journal_get_display_user_info($value_info);?>
				<?php print $user_info_tmp['display_picture'];?>
				</li>
				<?php endforeach;?>
				
				<li class="more-people"><a href="#">…</a></li>
			</ul>
		</li>
		<?php endforeach;?>
		
		<li class="journal-item">
			<div class="journal-status reject"><span>Reject</span></div>
			<h3 class="journal-title"><a href="#">Risk Factor For Developing First</a></h3>
			<div class="journal-update">
				<span class="label">Status update: </span>
				<span>13 Feb 2013</span>
			</div>
			<div class="journal-create">
				<span class="label">Create Date: </span>
				<span>1 Jan 2010</span>		
			</div>
			<ul class="relate-peoples">
				<li class="clickable-people owner profile-picture-small"><a href="#"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /><span>Owner</span></a></li>
				<li class="clickable-people"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="clickable-people"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
				<li class="clickable-people"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="clickable-people"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="clickable-people"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
				<li class="clickable-people"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
			</ul>
		</li>
	
		<li class="journal-item">
			<div class="journal-status finish-publish"><span>Finish Publish</span></div>
			<h3 class="journal-title"><a href="#">Risk Factor For Developing First</a></h3>
			<div class="journal-update">
				<span class="label">Status update: </span>
				<span>13 Feb 2013</span>
			</div>
			<div class="journal-create">
				<span class="label">Create Date: </span>
				<span>1 Jan 2010</span>		
			</div>
			<ul class="relate-peoples">
				<li><a href="#"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></a></li>
				<li class="clickable-people"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="clickable-people"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
				<li class="clickable-people"><img  class="profile-picture-small" src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
			</ul>
		</li>
	
		<li class="journal-item">
			<div class="journal-status information-approve"><span>Information Approve</span></div>
			<h3 class="journal-title"><a href="#">Risk Factor For Developing First</a></h3>
			<div class="journal-update">
				<span class="label">Status update: </span>
				<span>13 Feb 2013</span>
			</div>
			<div class="journal-create">
				<span class="label">Create Date: </span>
				<span>1 Jan 2010</span>		
			</div>
			<ul class="relate-peoples">
				<li class="clickable-people owner profile-picture-small"><a href="#"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /><span>Owner</span></a></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
				<li class="more-people">…</li>
			</ul>
		</li>
	
		<li class="journal-item">
			<div class="journal-status information-approve">Information Approve</div>
			<h3 class="journal-title"><a href="#">Risk Factor For Developing First</a></h3>
			<div class="journal-update">
				<span class="label">Status update: </span>
				<span>13 Feb 2013</span>
			</div>
			<div class="journal-create">
				<span class="label">Create Date: </span>
				<span>1 Jan 2010</span>		
			</div>
			<ul class="relate-peoples">
				<li class="clickable-people owner profile-picture-small"><a href="#"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /><span>Owner</span></a></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-chatchai.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="profile-picture-small"><img src="/sites/all/themes/clean_journal/images/dummy/small-picture-profile-tavee.png" alt="" /></li>
				<li class="more-people">…</li>
			</ul>
		</li>
		
	</ul>
	<div class="load-more"><a href="#">Load More …</a></div>
</div>