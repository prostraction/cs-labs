$(document).ready(function() {
	var SataCount = 0;
	var M2Count = 0;
	insert_new_conf();
	alert("111");
	
	display_item("#test_ram", '#show_ram', "show_ram");
	display_item("#test_gpu", '#show_gpu', "show_gpu");
	display_item2("#value_motherboard", "show_motherboard");
	display_item2("#value_cpu", "show_cpu");
	display_item2("#value_cpu_fan", "show_cpu_fan");
	display_item2("#value_psu", "show_psu");
	display_item2("#value_pc_case", "show_pc_case");
	
	
	$("#search_motherboard").keyup(function() {
		display_item("#display_motherboard", '#search_motherboard', "search_motherboard");
	});
	$("#search_cpu").keyup(function() {
		display_item("#display_cpu", '#search_cpu', "search_cpu");
	});
	$("#search_cpu_fan").keyup(function() {
		display_item("#display_cpu_fan", '#search_cpu_fan', "search_cpu_fan");
	});
	$("#search_ram").keyup(function() {
		display_item("#display_ram", '#search_ram', "search_ram");
		//display_item("#test_ram", '#show_ram', "show_ram");
	});
	$("#sata_count").bind('keyup mouseup', function () {
		if ($(sata_count).val() > 0 && $(sata_count).val() != SataCount) {
			SataCount = $(sata_count).val();
			alert(SataCount);
		}
		
	});
	$("#m2_count").bind('keyup mouseup', function () {
		if ($(m2_count).val() > 0 && $(m2_count).val() != M2Count) {
			M2Count = $(m2_count).val();
			alert(M2Count);
		}
	});
	$("#search_gpu").keyup(function() {
		display_item("#display_gpu", '#search_gpu', "search_gpu");
		//display_item("#test_gpu", '#show_gpu', "show_gpu");
	});
	$("#search_psu").keyup(function() {
		display_item("#display_psu", '#search_psu', "search_psu");
	});
	$("#search_pc_case").keyup(function() {
		display_item("#display_pc_case", '#search_pc_case', "search_pc_case");
	});
});
 
function fill(Key, Value, Display, Search, Action, ShowAction) {
	$(Search).val('');
    $(Display).hide();
	update_display_item(Action, Value, Key, ShowAction);
}

function update_display_item(Action, ValueId, Key, ShowAction) {
	var PostedValue = $(ValueId).val();
	if (PostedValue !== "") {
        $.ajax({
            type: "POST",
            url: "handler.php",
            data: Action+'='+ValueId,
			success: function(response) {
                    display_item2(Key, ShowAction);
            }
        });
    }
}

function insert_new_conf() {
	$.ajax({
            type: "POST",
            url: "handler.php",
            data: {configuration: 'new_configuration'}
    });
}

function fill_computer_ram(Key, ValueId, ValueName, Display, Search) {
	var PostedValue = $(ValueId).val();
	$(Search).val('');
    $(Display).hide();
	if (PostedValue !== "") {
        $.ajax({
            type: "POST",
            url: "handler.php",
            data:{action:'insert_ram', argument_ram_id: ValueId},
			success: function(response) {
                    display_item("#test_ram", '#show_ram', "show_ram");
            }
        });
    }
}

function delete_computer_ram(Key, ValueId) {
	var PostedValue = $(ValueId).val();
	if (PostedValue !== "") {
        $.ajax({
            type: "POST",
            url: "handler.php",
            data:{action:'delete_ram', argument_ram_id: ValueId},
			success: function(response) {
                    display_item("#test_ram", '#show_ram', "show_ram");
            }
        });
    }
}

function fill_computer_gpu(Key, ValueId, ValueName, Display, Search) {
	var PostedValue = $(ValueId).val();
	$(Search).val('');
    $(Display).hide();
	if (PostedValue !== "") {
        $.ajax({
            type: "POST",
            url: "handler.php",
            data:{action:'insert_gpu', argument_gpu_id: ValueId},
			success: function(response) {
                    display_item("#test_gpu", '#show_gpu', "show_gpu");
            }
        });
    }
}

function delete_computer_gpu(Key, ValueId) {
	var PostedValue = $(ValueId).val();
	if (PostedValue !== "") {
        $.ajax({
            type: "POST",
            url: "handler.php",
            data:{action:'delete_gpu', argument_gpu_id: ValueId},
			success: function(response) {
                    display_item("#test_gpu", '#show_gpu', "show_gpu");
            }
        });
    }
}

function display_item(Display, Search, Search_Object) {
        var PostedValue = $(Search).val();
        if (PostedValue === "") {
            $(Display).html("");
        }
        else {
            $.ajax({
                type: "POST",
                url: "handler.php",
                data: Search_Object+'='+PostedValue,
                success: function(response) {
                    $(Display).html(response).show();
                }
            });
        }
}

function display_item2(Display, Search_Object) {
	$.ajax({
            type: "POST",
            url: "handler.php",
            data: {action: Search_Object},
            success: function(response) {
				$(Display).html(response).show();
        }
    });
}