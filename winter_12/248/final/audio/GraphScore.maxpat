{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 0.0, 44.0, 1542.0, 981.0 ],
		"bglocked" : 0,
		"defrect" : [ 0.0, 44.0, 1542.0, 981.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi 27 0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1281.0, 262.0, 63.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-123",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi 27 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1280.0, 234.0, 63.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-121",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi 22 0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1280.0, 204.0, 63.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-102",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi 22 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1280.0, 176.0, 63.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-90",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 214.0, 745.0, 21.0, 21.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-79"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "open",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"patching_rect" : [ 174.0, 747.0, 34.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-85",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sfrecord~",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 126.0, 796.0, 61.0, 20.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-75",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "this is very important for audio playback!",
					"linecount" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 699.0, 204.0, 150.0, 34.0 ],
					"id" : "obj-130",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read hanning_extended.wav",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 695.0, 239.0, 161.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-124",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print /new",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 15.0, 713.115723, 63.0, 20.0 ],
					"id" : "obj-103",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r new",
					"numinlets" : 0,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 15.0, 635.0, 40.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-101",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s new",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 231.0, 370.0, 42.0, 20.0 ],
					"id" : "obj-92",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/new",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 15.0, 669.0, 36.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-91",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "send on",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 219.0, 177.0, 53.0, 20.0 ],
					"id" : "obj-83",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"patching_rect" : [ 72.0, 383.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-78",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"patching_rect" : [ 60.0, 409.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-60",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 93.0, 480.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-61",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "unpack",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 93.0, 436.0, 49.0, 20.0 ],
					"outlettype" : [ "int", "int" ],
					"id" : "obj-15",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "del 300",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1124.0, 317.0, 50.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-100",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r wipe",
					"numinlets" : 0,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 557.0, 19.0, 43.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-53",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s wipe",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 1124.0, 342.0, 45.0, 20.0 ],
					"id" : "obj-51",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi 20 0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1206.0, 276.718994, 63.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-97",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi 20 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1206.0, 252.718994, 63.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-95",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi -1 0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1206.0, 171.718994, 60.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-80",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi 23 0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1206.0, 228.718994, 63.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-50",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s topoly_R",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 1171.0, 593.0, 67.0, 20.0 ],
					"id" : "obj-35",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s topoly_L",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 1077.0, 593.0, 63.0, 20.0 ],
					"id" : "obj-44",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 1013.0, 507.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-28",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 1028.0, 427.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-27",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 1173.0, 432.0, 20.0, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-16"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "+ 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1084.0, 465.0, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-149",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b i",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 1084.0, 494.0, 32.5, 20.0 ],
					"outlettype" : [ "bang", "int" ],
					"id" : "obj-148",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 1173.0, 403.0, 20.0, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-146"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print to_L",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 1005.0, 593.0, 61.0, 20.0 ],
					"id" : "obj-104",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print to_R",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 1241.0, 593.0, 63.0, 20.0 ],
					"id" : "obj-120",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "int",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1171.0, 549.0, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-132",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi 23 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1206.0, 202.0, 63.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-133",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 1028.0, 403.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-138",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "unpack 0 0",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 1084.0, 370.0, 69.0, 20.0 ],
					"outlettype" : [ "int", "int" ],
					"id" : "obj-139",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route /midi /wipe",
					"numinlets" : 1,
					"numoutlets" : 3,
					"fontsize" : 12.0,
					"patching_rect" : [ 1084.0, 292.0, 98.0, 20.0 ],
					"outlettype" : [ "", "", "" ],
					"id" : "obj-141",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "target $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"patching_rect" : [ 1077.0, 549.0, 54.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-143",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "udpreceive 7001",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 1084.124023, 190.718994, 100.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-144",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 591.0, 696.0, 80.0, 13.0 ],
					"outlettype" : [ "float" ],
					"id" : "obj-129"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print to_L",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 728.0, 659.0, 61.0, 20.0 ],
					"id" : "obj-128",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print to_R",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 957.0, 659.0, 63.0, 20.0 ],
					"id" : "obj-127",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "== 0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 848.0, 526.0, 35.0, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-110",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pack 0 0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 805.0, 551.0, 62.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-111",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "mute $1 $2",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"patching_rect" : [ 804.666687, 577.0, 69.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-116",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t i i",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 791.0, 483.0, 32.5, 20.0 ],
					"outlettype" : [ "int", "int" ],
					"id" : "obj-84",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "1000 ms * 60 sec",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 368.0, 178.0, 101.0, 20.0 ],
					"id" : "obj-74",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s topoly_R",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 630.0, 176.0, 67.0, 20.0 ],
					"id" : "obj-68",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r topoly_R",
					"numinlets" : 0,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 611.0, 623.0, 65.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-63",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b i",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 817.0, 426.0, 32.5, 20.0 ],
					"outlettype" : [ "bang", "int" ],
					"id" : "obj-59",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 913.0, 444.0, 33.0, 33.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-57"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "int",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 885.0, 526.0, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-52",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/midi 27 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 942.0, 272.718994, 63.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-33",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s topoly_R",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 885.0, 659.0, 67.0, 20.0 ],
					"id" : "obj-14",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t i i",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 702.0, 147.0, 32.5, 20.0 ],
					"outlettype" : [ "int", "int" ],
					"id" : "obj-76",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s out",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 508.0, 696.0, 37.0, 20.0 ],
					"id" : "obj-73",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r topoly_L",
					"numinlets" : 0,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 508.0, 623.0, 63.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-72",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "poly~ note_grains 48",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 508.0, 662.0, 122.0, 20.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-71",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 653.0, 239.0, 35.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-70",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ envelope",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 653.0, 268.0, 100.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-11",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "to get to -1",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 191.0, 564.0, 66.0, 20.0 ],
					"id" : "obj-69",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "+ 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 257.0, 612.0, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-64",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s topoly_L",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 257.0, 672.115723, 63.0, 20.0 ],
					"id" : "obj-65",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "target $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"patching_rect" : [ 257.0, 642.0, 54.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-66",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b f b",
					"numinlets" : 1,
					"numoutlets" : 3,
					"fontsize" : 12.0,
					"patching_rect" : [ 430.0, 412.0, 46.0, 20.0 ],
					"outlettype" : [ "bang", "float", "bang" ],
					"id" : "obj-62",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t i i",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 165.0, 395.0, 32.5, 20.0 ],
					"outlettype" : [ "int", "int" ],
					"id" : "obj-58",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "18 20",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 177.0, 370.0, 41.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-54",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 755.0, 408.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-49",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s topoly_L",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 557.0, 239.0, 63.0, 20.0 ],
					"id" : "obj-46",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "target $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"patching_rect" : [ 557.0, 206.0, 54.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-48",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 557.0, 46.0, 31.0, 31.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-45"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b b b",
					"numinlets" : 1,
					"numoutlets" : 3,
					"fontsize" : 12.0,
					"patching_rect" : [ 557.0, 84.0, 164.0, 20.0 ],
					"outlettype" : [ "bang", "bang", "bang" ],
					"id" : "obj-29",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "+ 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 791.0, 456.0, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-10",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "$1 50",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 152.0, 181.0, 41.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-118",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "line~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 152.0, 204.0, 36.0, 20.0 ],
					"outlettype" : [ "signal", "bang" ],
					"id" : "obj-5",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p mono_keys",
					"numinlets" : 0,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 10.0, 171.0, 82.0, 20.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-122",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 2.0, 44.0, 514.0, 441.0 ],
						"bglocked" : 0,
						"defrect" : [ 2.0, 44.0, 514.0, 441.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print",
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 322.0, 234.0, 34.0, 20.0 ],
									"id" : "obj-10",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "* 4",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 390.0, 106.0, 32.5, 20.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-9",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "+ 50",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 389.0, 131.0, 34.0, 20.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-6",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0 0",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 266.0, 151.0, 56.0, 20.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-1",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number~",
									"numinlets" : 2,
									"mode" : 2,
									"sig" : 0.0,
									"numoutlets" : 2,
									"fontsize" : 11.595187,
									"patching_rect" : [ 263.0, 273.0, 47.0, 20.0 ],
									"outlettype" : [ "signal", "float" ],
									"id" : "obj-121",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "0. 10 0.9 20 0.6 50 0.6 $2 0 $2",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 241.0, 186.0, 174.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-15",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "line~",
									"numinlets" : 2,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 241.0, 220.0, 36.0, 20.0 ],
									"outlettype" : [ "signal", "bang" ],
									"id" : "obj-13",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 183.0, 275.0, 32.5, 20.0 ],
									"outlettype" : [ "signal" ],
									"id" : "obj-12",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 86.0, 229.0, 32.5, 20.0 ],
									"outlettype" : [ "signal" ],
									"id" : "obj-5",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 3",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 159.0, 175.0, 55.0, 20.0 ],
									"outlettype" : [ "signal" ],
									"id" : "obj-3",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 178.0, 377.0, 25.0, 25.0 ],
									"id" : "obj-8",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "kslider",
									"numinlets" : 2,
									"presentation_rect" : [ 30.0, 30.0, 348.0, 53.0 ],
									"numoutlets" : 2,
									"patching_rect" : [ 86.0, 42.0, 348.0, 53.0 ],
									"outlettype" : [ "int", "int" ],
									"offset" : 47,
									"range" : 49,
									"id" : "obj-16"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 440",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 86.0, 176.0, 68.0, 20.0 ],
									"outlettype" : [ "signal" ],
									"id" : "obj-7",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "gain~",
									"numinlets" : 2,
									"numoutlets" : 2,
									"patching_rect" : [ 180.0, 307.0, 38.0, 50.0 ],
									"outlettype" : [ "signal", "int" ],
									"id" : "obj-2"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "mtof",
									"numinlets" : 1,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 86.0, 143.0, 34.0, 20.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-4",
									"fontname" : "Arial"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-16", 1 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-1", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-121", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-12", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-5", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontsize" : 12.0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontface" : 0,
						"fontname" : "Arial",
						"default_fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 911.0, 417.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-119",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 755.0, 387.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-117",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "unpack 0 0",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 817.0, 375.0, 101.0, 20.0 ],
					"outlettype" : [ "int", "int" ],
					"id" : "obj-115",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print /midi",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 835.0, 352.0, 63.0, 20.0 ],
					"id" : "obj-113",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route /midi",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 817.0, 317.0, 67.0, 20.0 ],
					"outlettype" : [ "", "" ],
					"id" : "obj-99",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 58.785126, 90.603333, 80.0, 13.0 ],
					"outlettype" : [ "float" ],
					"warmcolor" : [ 0.85098, 0.85098, 0.0, 1.0 ],
					"tepidcolor" : [ 0.6, 0.729412, 0.0, 1.0 ],
					"bgcolor" : [ 0.403922, 0.403922, 0.403922, 1.0 ],
					"id" : "obj-81",
					"coldcolor" : [ 0.0, 0.658824, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 219.0, 121.0, 35.0, 35.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-56"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 138.0, 238.0, 32.5, 20.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-20",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "-2",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"patching_rect" : [ 256.75, 564.0, 32.5, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-105",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "int",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 257.0, 587.0, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-77",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "togedge",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 222.0, 540.0, 54.0, 20.0 ],
					"outlettype" : [ "bang", "bang" ],
					"id" : "obj-12",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 630.0, 145.0, 37.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-93",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 702.0, 118.0, 32.5, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-89",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b f",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 392.0, 411.0, 32.5, 20.0 ],
					"outlettype" : [ "bang", "float" ],
					"id" : "obj-87",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "-1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"patching_rect" : [ 556.75, 118.0, 29.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-32",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s topoly_L",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 791.0, 659.0, 63.0, 20.0 ],
					"id" : "obj-34",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "target $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"patching_rect" : [ 791.0, 526.0, 54.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-55",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pack 0. 0.",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 471.0, 592.0, 63.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-88",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "fswap",
					"numinlets" : 2,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 471.0, 563.0, 42.0, 20.0 ],
					"outlettype" : [ "int", "int" ],
					"id" : "obj-86",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "float",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 444.0, 454.0, 33.0, 20.0 ],
					"outlettype" : [ "float" ],
					"id" : "obj-19",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "float",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 444.0, 530.0, 33.0, 20.0 ],
					"outlettype" : [ "float" ],
					"id" : "obj-21",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t f b f b",
					"numinlets" : 1,
					"numoutlets" : 4,
					"fontsize" : 12.0,
					"patching_rect" : [ 444.0, 485.0, 59.5, 20.0 ],
					"outlettype" : [ "float", "bang", "float", "bang" ],
					"id" : "obj-30",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 496.0, 302.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-114",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "- 40.",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 458.0, 328.0, 35.0, 20.0 ],
					"outlettype" : [ "float" ],
					"id" : "obj-112",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 344.239624, 383.0, 34.0, 20.0 ],
					"id" : "obj-109",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 1.",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 458.0, 303.0, 32.5, 20.0 ],
					"outlettype" : [ "float" ],
					"id" : "obj-106",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pack text 1.",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 392.0, 353.0, 85.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-107",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "unpack text 1.",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 392.0, 278.0, 85.0, 20.0 ],
					"outlettype" : [ "", "float" ],
					"id" : "obj-108",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route start change stop",
					"numinlets" : 1,
					"numoutlets" : 4,
					"fontsize" : 12.0,
					"patching_rect" : [ 392.0, 383.0, 133.0, 20.0 ],
					"outlettype" : [ "", "", "", "" ],
					"id" : "obj-94",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"patching_rect" : [ 52.727272, 317.909088, 38.0, 50.0 ],
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-96"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 165.347107, 541.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-82",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r out",
					"numinlets" : 0,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 55.909088, 285.0, 35.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-47",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "> 3",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 165.0, 506.0, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-67",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 205.0, 506.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-42",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 180.0, 448.0, 32.5, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-41",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "int",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 165.0, 475.0, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-39",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "+ 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 204.0, 475.0, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-31",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 472.0, 118.0, 60.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-6",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"numinlets" : 2,
					"mode" : 2,
					"sig" : 0.0,
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"patching_rect" : [ 159.0, 295.0, 91.0, 20.0 ],
					"outlettype" : [ "signal", "float" ],
					"id" : "obj-26",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "size $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 500.0, 239.0, 49.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-25",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 60000.",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 472.0, 178.0, 55.0, 20.0 ],
					"outlettype" : [ "float" ],
					"id" : "obj-24",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "30.",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 472.0, 147.0, 32.5, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-23",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "30 minutes",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 396.0, 147.0, 71.0, 20.0 ],
					"id" : "obj-13",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "record~ audiobuff",
					"numinlets" : 3,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 159.0, 265.0, 104.0, 20.0 ],
					"outlettype" : [ "signal" ],
					"id" : "obj-4",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ audiobuff 1200000",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 500.0, 268.0, 151.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-18",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "resume",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 224.0, 58.0, 52.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-36",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "pause",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 216.0, 37.0, 44.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-37",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezadc~",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 22.0, 30.0, 45.0, 45.0 ],
					"outlettype" : [ "signal", "signal" ],
					"id" : "obj-1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 60.0, 635.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-7",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"patching_rect" : [ 10.0, 317.0, 38.0, 50.0 ],
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-2"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print /note",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 156.0, 672.115723, 64.0, 20.0 ],
					"id" : "obj-9",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 193.0, 38.0, 19.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-38",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 172.0, 38.0, 19.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-40",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "open",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 132.0, 38.0, 38.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-8",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sfplay~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"patching_rect" : [ 138.0, 68.0, 50.0, 20.0 ],
					"outlettype" : [ "signal", "bang" ],
					"id" : "obj-43",
					"fontname" : "Arial",
					"save" : [ "#N", "sfplay~", "", 1, 120960, 0, "", ";" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "udpreceive 7001",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 817.124023, 272.718994, 100.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-3",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 167.785126, 101.603333, 80.0, 13.0 ],
					"outlettype" : [ "float" ],
					"warmcolor" : [ 0.85098, 0.85098, 0.0, 1.0 ],
					"tepidcolor" : [ 0.6, 0.729412, 0.0, 1.0 ],
					"bgcolor" : [ 0.403922, 0.403922, 0.403922, 1.0 ],
					"id" : "obj-17",
					"coldcolor" : [ 0.0, 0.658824, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p listen",
					"numinlets" : 2,
					"numoutlets" : 3,
					"fontsize" : 12.0,
					"patching_rect" : [ 137.785156, 332.603333, 112.0, 20.0 ],
					"outlettype" : [ "", "", "" ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 3.0, 146.0, 798.0, 773.0 ],
						"bglocked" : 0,
						"defrect" : [ 3.0, 146.0, 798.0, 773.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 755.0, 506.0, 25.0, 25.0 ],
									"id" : "obj-96",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 372.0, 642.0, 20.0, 20.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-95"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 338.0, 642.0, 20.0, 20.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-93"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "togedge",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 327.0, 619.0, 54.0, 20.0 ],
									"outlettype" : [ "bang", "bang" ],
									"id" : "obj-91",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 379.0, 597.0, 50.0, 20.0 ],
									"outlettype" : [ "int", "bang" ],
									"id" : "obj-90",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "something absurd",
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 364.0, 665.0, 119.0, 20.0 ],
									"id" : "obj-87",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "-3",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 327.0, 665.0, 32.5, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-85",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "> 4000",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 327.0, 596.0, 48.0, 20.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-82",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "sel 1",
									"numinlets" : 2,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 378.0, 513.0, 36.0, 20.0 ],
									"outlettype" : [ "bang", "" ],
									"id" : "obj-79",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b",
									"numinlets" : 1,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 327.0, 545.0, 24.0, 20.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-71",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 418.0, 513.0, 20.0, 20.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-68"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "r on",
									"numinlets" : 0,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 418.0, 487.0, 31.0, 20.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-65",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "metro 500",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 418.0, 540.0, 65.0, 20.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-57",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "timer @format bbu",
									"numinlets" : 2,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 327.0, 572.0, 110.0, 20.0 ],
									"outlettype" : [ "float", "" ],
									"id" : "obj-53",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "100",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 26.0, 495.0, 32.5, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-21",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 36.0, 471.0, 50.0, 20.0 ],
									"outlettype" : [ "int", "bang" ],
									"id" : "obj-17",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 29.0, 447.0, 50.0, 20.0 ],
									"outlettype" : [ "int", "bang" ],
									"id" : "obj-9",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 19.0, 419.0, 50.0, 20.0 ],
									"outlettype" : [ "int", "bang" ],
									"id" : "obj-8",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t i i",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 349.0, 278.0, 32.5, 20.0 ],
									"outlettype" : [ "int", "int" ],
									"id" : "obj-19",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "> 0",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 438.0, 328.0, 32.5, 20.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-80",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 381.0, 354.0, 50.0, 20.0 ],
									"outlettype" : [ "int", "bang" ],
									"id" : "obj-74",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "togedge",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 438.0, 354.0, 54.0, 20.0 ],
									"outlettype" : [ "bang", "bang" ],
									"id" : "obj-66",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "float",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 617.0, 434.0, 33.0, 20.0 ],
									"outlettype" : [ "float" ],
									"id" : "obj-63",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "float",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 560.0, 434.0, 33.0, 20.0 ],
									"outlettype" : [ "float" ],
									"id" : "obj-62",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "float",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 487.0, 434.0, 33.0, 20.0 ],
									"outlettype" : [ "float" ],
									"id" : "obj-61",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "stop $1",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 617.0, 460.0, 50.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-51",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "change $1",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 487.0, 460.0, 67.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-49",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "start $1",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 560.0, 460.0, 51.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-11",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "split 0 127",
									"numinlets" : 3,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 363.0, 328.0, 65.0, 18.0 ],
									"outlettype" : [ "int", "int" ],
									"id" : "obj-64",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 145.0, 437.0, 50.0, 20.0 ],
									"outlettype" : [ "int", "bang" ],
									"id" : "obj-48",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 560.0, 506.0, 25.0, 25.0 ],
									"id" : "obj-60",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 210.0, 633.0, 25.0, 25.0 ],
									"id" : "obj-59",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 574.0, 342.0, 25.0, 25.0 ],
									"outlettype" : [ "float" ],
									"id" : "obj-23",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 144.0, 20.0, 25.0, 25.0 ],
									"outlettype" : [ "signal" ],
									"id" : "obj-22",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 198.0, 437.0, 20.0, 20.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-20"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p threeInARow",
									"numinlets" : 1,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 198.0, 376.0, 90.0, 20.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-58",
									"fontname" : "Arial",
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 633.0, 52.0, 155.0, 405.0 ],
										"bglocked" : 0,
										"defrect" : [ 633.0, 52.0, 155.0, 405.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 0,
										"toolbarvisible" : 1,
										"boxanimatetime" : 200,
										"imprint" : 0,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "gate",
													"numinlets" : 2,
													"numoutlets" : 1,
													"fontsize" : 12.0,
													"patching_rect" : [ 26.0, 297.0, 34.0, 20.0 ],
													"outlettype" : [ "" ],
													"id" : "obj-8",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "==",
													"numinlets" : 2,
													"numoutlets" : 1,
													"fontsize" : 12.0,
													"patching_rect" : [ 26.0, 266.0, 32.5, 20.0 ],
													"outlettype" : [ "int" ],
													"id" : "obj-9",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "int",
													"numinlets" : 2,
													"numoutlets" : 1,
													"fontsize" : 12.0,
													"patching_rect" : [ 41.0, 228.0, 32.5, 20.0 ],
													"outlettype" : [ "int" ],
													"id" : "obj-10",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t i i i b",
													"numinlets" : 1,
													"numoutlets" : 4,
													"fontsize" : 12.0,
													"patching_rect" : [ 47.0, 67.0, 59.5, 20.0 ],
													"outlettype" : [ "int", "int", "int", "bang" ],
													"id" : "obj-55",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "gate",
													"numinlets" : 2,
													"numoutlets" : 1,
													"fontsize" : 12.0,
													"patching_rect" : [ 78.0, 194.0, 34.0, 20.0 ],
													"outlettype" : [ "" ],
													"id" : "obj-51",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "==",
													"numinlets" : 2,
													"numoutlets" : 1,
													"fontsize" : 12.0,
													"patching_rect" : [ 78.0, 163.0, 32.5, 20.0 ],
													"outlettype" : [ "int" ],
													"id" : "obj-23",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "int",
													"numinlets" : 2,
													"numoutlets" : 1,
													"fontsize" : 12.0,
													"patching_rect" : [ 93.0, 125.0, 32.5, 20.0 ],
													"outlettype" : [ "int" ],
													"id" : "obj-22",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 26.0, 335.0, 25.0, 25.0 ],
													"id" : "obj-1",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"patching_rect" : [ 47.0, 26.0, 25.0, 25.0 ],
													"outlettype" : [ "int" ],
													"id" : "obj-4",
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-55", 1 ],
													"destination" : [ "obj-9", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-55", 0 ],
													"destination" : [ "obj-8", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-55", 3 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-51", 0 ],
													"destination" : [ "obj-10", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-9", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-55", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-22", 0 ],
													"destination" : [ "obj-23", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-23", 0 ],
													"destination" : [ "obj-51", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-55", 3 ],
													"destination" : [ "obj-22", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-55", 2 ],
													"destination" : [ "obj-22", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-55", 1 ],
													"destination" : [ "obj-23", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-55", 0 ],
													"destination" : [ "obj-51", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"globalpatchername" : "",
										"default_fontsize" : 12.0,
										"fontface" : 0,
										"fontsize" : 12.0,
										"default_fontface" : 0,
										"fontname" : "Arial",
										"default_fontname" : "Arial"
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "This could be changed for transposing instruments",
									"linecount" : 2,
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 39.0, 291.0, 150.0, 30.0 ],
									"id" : "obj-15",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "this maps midi 47 to 0",
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 39.0, 271.0, 150.0, 18.0 ],
									"id" : "obj-14",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "- 47",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 198.0, 289.0, 35.0, 18.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-13",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "47 is the maximum number of notes representable in my system",
									"linecount" : 2,
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 5.0, 338.0, 194.0, 30.0 ],
									"id" : "obj-6",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "split 0 47",
									"numinlets" : 3,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 198.0, 348.0, 59.0, 18.0 ],
									"outlettype" : [ "int", "int" ],
									"id" : "obj-12",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 1 0 127",
									"numinlets" : 6,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 349.198364, 256.289246, 94.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-10",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"triscale" : 0.9,
									"patching_rect" : [ 365.826447, 227.760315, 93.0, 18.0 ],
									"outlettype" : [ "float", "bang" ],
									"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
									"minimum" : 0.0,
									"maximum" : 1.0,
									"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
									"id" : "obj-1",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 1 1",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 210.0, 605.0, 61.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-2",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "fswap 1",
									"numinlets" : 2,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"patching_rect" : [ 210.0, 579.0, 61.0, 18.0 ],
									"outlettype" : [ "int", "int" ],
									"id" : "obj-3",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 1 1",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 210.0, 554.0, 58.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-4",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "change",
									"numinlets" : 1,
									"numoutlets" : 3,
									"fontsize" : 12.0,
									"patching_rect" : [ 197.628113, 405.471069, 52.0, 18.0 ],
									"outlettype" : [ "", "int", "int" ],
									"id" : "obj-24",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"triscale" : 0.9,
									"patching_rect" : [ 220.628098, 248.471069, 38.0, 18.0 ],
									"outlettype" : [ "int", "bang" ],
									"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
									"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
									"id" : "obj-25",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ftom",
									"numinlets" : 1,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 197.628098, 225.471069, 40.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-26",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "speedlim 60",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 348.826447, 173.760315, 75.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-28",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 40 80 0. 1.",
									"numinlets" : 6,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 348.826447, 197.760315, 130.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-29",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"triscale" : 0.9,
									"patching_rect" : [ 213.628098, 205.471069, 90.0, 18.0 ],
									"outlettype" : [ "float", "bang" ],
									"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
									"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
									"id" : "obj-30",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "mtof",
									"numinlets" : 1,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 196.628098, 164.471069, 37.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-31",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Print attack •",
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 15.628098, 142.471069, 93.0, 18.0 ],
									"id" : "obj-32",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 112.628098, 140.471069, 15.0, 15.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-33"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print pitch",
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 111.628098, 179.471069, 72.0, 18.0 ],
									"id" : "obj-34",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "gate",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 111.628098, 159.471069, 35.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-35",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "gate",
									"numinlets" : 2,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 119.628098, 227.471069, 35.0, 18.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-36",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print attack",
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 119.628098, 247.471069, 79.0, 18.0 ],
									"id" : "obj-37",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 120.628098, 208.471069, 15.0, 15.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-38"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Print attack •",
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 23.628098, 210.471069, 93.0, 18.0 ],
									"id" : "obj-39",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "(up to 3 outputs)",
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 231.628098, 188.471069, 111.0, 18.0 ],
									"id" : "obj-40",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"triscale" : 0.9,
									"patching_rect" : [ 219.628098, 130.471069, 87.0, 18.0 ],
									"outlettype" : [ "float", "bang" ],
									"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
									"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
									"triangle" : 0,
									"id" : "obj-42",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "unpack 0.",
									"numinlets" : 1,
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"patching_rect" : [ 196.628098, 95.471077, 68.0, 18.0 ],
									"outlettype" : [ "float" ],
									"id" : "obj-43",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "fiddle~ 2048 1 20 3",
									"numinlets" : 1,
									"numoutlets" : 5,
									"fontsize" : 12.0,
									"patching_rect" : [ 143.628098, 58.471077, 240.0, 18.0 ],
									"outlettype" : [ "float", "bang", "list", "float", "list" ],
									"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
									"id" : "obj-44",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "raw pitch and amplitude",
									"linecount" : 2,
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 232.628098, 161.471069, 108.0, 30.0 ],
									"id" : "obj-45",
									"fontname" : "Helvetica"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "amplitude (dB)",
									"linecount" : 2,
									"numinlets" : 1,
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"patching_rect" : [ 348.826447, 141.760315, 48.0, 30.0 ],
									"id" : "obj-46",
									"fontname" : "Helvetica"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-82", 0 ],
									"destination" : [ "obj-91", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-53", 0 ],
									"destination" : [ "obj-82", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-85", 0 ],
									"destination" : [ "obj-96", 0 ],
									"hidden" : 0,
									"midpoints" : [ 336.5, 688.0, 734.0, 688.0, 734.0, 460.0, 764.5, 460.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-91", 0 ],
									"destination" : [ "obj-85", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-79", 0 ],
									"destination" : [ "obj-71", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-91", 1 ],
									"destination" : [ "obj-95", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-91", 0 ],
									"destination" : [ "obj-93", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-53", 0 ],
									"destination" : [ "obj-90", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-85", 0 ],
									"destination" : [ "obj-4", 1 ],
									"hidden" : 0,
									"midpoints" : [ 336.5, 688.0, 304.0, 688.0, 304.0, 541.0, 258.5, 541.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-57", 0 ],
									"destination" : [ "obj-53", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-68", 0 ],
									"destination" : [ "obj-57", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-65", 0 ],
									"destination" : [ "obj-79", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-71", 0 ],
									"destination" : [ "obj-53", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-64", 0 ],
									"destination" : [ "obj-71", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-71", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-65", 0 ],
									"destination" : [ "obj-68", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-64", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 1 ],
									"destination" : [ "obj-64", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-80", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-51", 0 ],
									"destination" : [ "obj-60", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-60", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-49", 0 ],
									"destination" : [ "obj-60", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-80", 0 ],
									"destination" : [ "obj-66", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-64", 0 ],
									"destination" : [ "obj-74", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-66", 0 ],
									"destination" : [ "obj-62", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-66", 1 ],
									"destination" : [ "obj-63", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-61", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-63", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-62", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-61", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-62", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-63", 0 ],
									"destination" : [ "obj-51", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-61", 0 ],
									"destination" : [ "obj-49", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-44", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-29", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-44", 2 ],
									"destination" : [ "obj-43", 0 ],
									"hidden" : 0,
									"midpoints" : [ 263.628113, 85.471069, 206.128098, 85.471069 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-44", 1 ],
									"destination" : [ "obj-36", 1 ],
									"hidden" : 0,
									"midpoints" : [ 208.378098, 79.471069, 188.628098, 79.471069, 188.628098, 213.471069, 145.128098, 213.471069 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-44", 0 ],
									"destination" : [ "obj-35", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-58", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-58", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-33", 0 ],
									"destination" : [ "obj-35", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-35", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-38", 0 ],
									"destination" : [ "obj-36", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-36", 0 ],
									"destination" : [ "obj-37", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-43", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-31", 0 ],
									"destination" : [ "obj-26", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 0 ],
									"destination" : [ "obj-25", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-31", 0 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-43", 0 ],
									"destination" : [ "obj-42", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-44", 3 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-4", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 1 ],
									"destination" : [ "obj-2", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-59", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-48", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [ 80.0, 442.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-58", 0 ],
									"destination" : [ "obj-4", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontsize" : 12.0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontface" : 0,
						"fontname" : "Arial",
						"default_fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend /note",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 137.785156, 642.0, 86.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-98",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend /test",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"patching_rect" : [ 59.52066, 669.0, 82.0, 20.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-126",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 22.785126, 445.603302, 33.0, 33.0 ],
					"id" : "obj-155"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "udpsend localhost 7000",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 138.173615, 713.115723, 138.0, 20.0 ],
					"id" : "obj-159",
					"fontname" : "Arial"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-75", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-85", 0 ],
					"destination" : [ "obj-75", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-79", 0 ],
					"destination" : [ "obj-75", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-124", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-124", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-91", 0 ],
					"destination" : [ "obj-103", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-101", 0 ],
					"destination" : [ "obj-91", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 2 ],
					"destination" : [ "obj-92", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-91", 0 ],
					"destination" : [ "obj-159", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-83", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 1 ],
					"destination" : [ "obj-78", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 1 ],
					"destination" : [ "obj-60", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 2 ],
					"destination" : [ "obj-89", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 1 ],
					"destination" : [ "obj-93", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-143", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-132", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-143", 0 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-149", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-149", 0 ],
					"destination" : [ "obj-148", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-148", 1 ],
					"destination" : [ "obj-143", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-148", 0 ],
					"destination" : [ "obj-132", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-132", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-139", 1 ],
					"destination" : [ "obj-132", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 1 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 0,
					"midpoints" : [ 725.0, 201.0, 566.5, 201.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-89", 0 ],
					"destination" : [ "obj-76", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-65", 0 ],
					"hidden" : 0,
					"midpoints" : [ 266.5, 665.0, 266.5, 674.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-46", 0 ],
					"hidden" : 0,
					"midpoints" : [ 566.5, 228.0, 566.5, 237.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-72", 0 ],
					"destination" : [ "obj-71", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-73", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-88", 0 ],
					"destination" : [ "obj-71", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-70", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-87", 1 ],
					"destination" : [ "obj-21", 1 ],
					"hidden" : 0,
					"midpoints" : [ 415.0, 515.0, 467.5, 515.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-94", 0 ],
					"destination" : [ "obj-87", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-87", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-94", 1 ],
					"destination" : [ "obj-62", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 1 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 2 ],
					"destination" : [ "obj-77", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-94", 2 ],
					"destination" : [ "obj-62", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 0 ],
					"destination" : [ "obj-64", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-64", 0 ],
					"destination" : [ "obj-66", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-107", 0 ],
					"destination" : [ "obj-109", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-67", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-67", 0 ],
					"destination" : [ "obj-82", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-67", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-58", 1 ],
					"destination" : [ "obj-77", 1 ],
					"hidden" : 0,
					"midpoints" : [ 188.0, 428.0, 280.0, 428.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-58", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-54", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-39", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-81", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-106", 0 ],
					"destination" : [ "obj-112", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-112", 0 ],
					"destination" : [ "obj-107", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-114", 0 ],
					"destination" : [ "obj-112", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-122", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-20", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-118", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-118", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-105", 0 ],
					"destination" : [ "obj-77", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 1 ],
					"destination" : [ "obj-105", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 1 ],
					"destination" : [ "obj-108", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-86", 1 ],
					"destination" : [ "obj-88", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-86", 0 ],
					"destination" : [ "obj-88", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 2 ],
					"destination" : [ "obj-86", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-86", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 3 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-107", 0 ],
					"destination" : [ "obj-94", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-106", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-108", 1 ],
					"destination" : [ "obj-106", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-108", 0 ],
					"destination" : [ "obj-107", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-96", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 0 ],
					"destination" : [ "obj-155", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 0 ],
					"destination" : [ "obj-155", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 1 ],
					"destination" : [ "obj-22", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-98", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-98", 0 ],
					"destination" : [ "obj-159", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-155", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-155", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-126", 0 ],
					"destination" : [ "obj-159", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-126", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-99", 0 ],
					"destination" : [ "obj-113", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-115", 0 ],
					"destination" : [ "obj-117", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-115", 1 ],
					"destination" : [ "obj-119", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-99", 0 ],
					"destination" : [ "obj-115", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-99", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-52", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-115", 0 ],
					"destination" : [ "obj-59", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 1 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-52", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-59", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-98", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-63", 0 ],
					"destination" : [ "obj-71", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-93", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-93", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 639.5, 170.0, 625.0, 170.0, 625.0, 262.0, 509.5, 262.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 0 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-110", 0 ],
					"destination" : [ "obj-111", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-111", 0 ],
					"destination" : [ "obj-116", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-110", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-127", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-116", 0 ],
					"destination" : [ "obj-128", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-128", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 1 ],
					"destination" : [ "obj-111", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-129", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-116", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-115", 1 ],
					"destination" : [ "obj-52", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-115", 1 ],
					"destination" : [ "obj-110", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-139", 0 ],
					"destination" : [ "obj-149", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-139", 1 ],
					"destination" : [ "obj-146", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-139", 0 ],
					"destination" : [ "obj-138", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-141", 0 ],
					"destination" : [ "obj-139", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-144", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-133", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-80", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-95", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-97", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-53", 0 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-141", 1 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-100", 0 ],
					"destination" : [ "obj-51", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-90", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-102", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-121", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-123", 0 ],
					"destination" : [ "obj-141", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
