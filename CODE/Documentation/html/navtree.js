var NAVTREE =
[
  [ "Embedded GarageBand", "index.html", [
    [ "Data Structures", "annotated.html", [
      [ "basicRfCfg_t", "structbasic_rf_cfg__t.html", null ],
      [ "basicRfPktHdr_t", "structbasic_rf_pkt_hdr__t.html", null ],
      [ "basicRfRxInfo_t", "structbasic_rf_rx_info__t.html", null ],
      [ "basicRfTxState_t", "structbasic_rf_tx_state__t.html", null ],
      [ "digioConfig", "structdigio_config.html", null ],
      [ "Loc_DataPayload", "struct_loc___data_payload.html", null ],
      [ "menu_t", "structmenu__t.html", null ],
      [ "menuItem_t", "structmenu_item__t.html", null ],
      [ "Ref_DataPayload", "struct_ref___data_payload.html", null ],
      [ "TimerInst", "struct_timer_inst.html", null ]
    ] ],
    [ "Data Structure Index", "classes.html", null ],
    [ "Data Fields", "functions.html", null ],
    [ "File List", "files.html", [
      [ "adc.c", "adc_8c.html", null ],
      [ "adc.h", "adc_8h.html", null ],
      [ "App_RF.c", "_app___r_f_8c.html", null ],
      [ "App_RF.h", "_app___r_f_8h.html", null ],
      [ "basic_rf.c", "basic__rf_8c.html", null ],
      [ "basic_rf.h", "basic__rf_8h.html", null ],
      [ "clock.c", "clock_8c.html", null ],
      [ "clock.h", "clock_8h.html", null ],
      [ "Drum.c", "_drum_8c.html", null ],
      [ "Drum.h", "_drum_8h.html", null ],
      [ "graphics_lcd.c", "graphics__lcd_8c.html", null ],
      [ "graphics_lcd.h", "graphics__lcd_8h.html", null ],
      [ "hal_assert.c", "hal__assert_8c.html", null ],
      [ "hal_assert.h", "hal__assert_8h.html", null ],
      [ "hal_board.c", "hal__board_8c.html", null ],
      [ "hal_board.h", "hal__board_8h.html", null ],
      [ "hal_cc8051.h", "hal__cc8051_8h.html", null ],
      [ "hal_defs.h", "hal__defs_8h.html", null ],
      [ "hal_digio.c", "hal__digio_8c.html", null ],
      [ "hal_digio.h", "hal__digio_8h.html", null ],
      [ "hal_int.c", "hal__int_8c.html", null ],
      [ "hal_int.h", "hal__int_8h.html", null ],
      [ "hal_led.c", "hal__led_8c.html", null ],
      [ "hal_led.h", "hal__led_8h.html", null ],
      [ "hal_mcu.c", "hal__mcu_8c.html", null ],
      [ "hal_mcu.h", "hal__mcu_8h.html", null ],
      [ "hal_rf.c", "hal__rf_8c.html", null ],
      [ "hal_rf.h", "hal__rf_8h.html", null ],
      [ "hal_SPI.c", "hal___s_p_i_8c.html", null ],
      [ "hal_SPI.h", "hal___s_p_i_8h.html", null ],
      [ "hal_types.h", "hal__types_8h.html", null ],
      [ "kb.c", "kb_8c.html", null ],
      [ "kb.h", "kb_8h.html", null ],
      [ "key_map.c", "key__map_8c.html", null ],
      [ "key_map.h", "key__map_8h.html", null ],
      [ "Main_mod1.c", "_main__mod1_8c.html", null ],
      [ "Main_mod2.c", "_main__mod2_8c.html", null ],
      [ "Main_mod3.c", "_main__mod3_8c.html", null ],
      [ "midi.c", "midi_8c.html", null ],
      [ "midi.h", "midi_8h.html", null ],
      [ "mmc2.c", "mmc2_8c.html", null ],
      [ "mmc2.h", "mmc2_8h.html", null ],
      [ "piano.c", "piano_8c.html", null ],
      [ "piano.h", "piano_8h.html", null ],
      [ "pp_utils.h", "pp__utils_8h.html", null ],
      [ "Project.h", "_project_8h.html", null ],
      [ "scancodes.h", "scancodes_8h.html", null ],
      [ "TimerManager.c", "_timer_manager_8c.html", null ],
      [ "TimerManager.h", "_timer_manager_8h.html", null ],
      [ "TouchScreen.c", "_touch_screen_8c.html", null ],
      [ "TouchScreen.h", "_touch_screen_8h.html", null ],
      [ "uart.c", "uart_8c.html", null ],
      [ "uart.h", "uart_8h.html", null ],
      [ "uart_intfc.c", "uart__intfc_8c.html", null ],
      [ "uart_intfc.h", "uart__intfc_8h.html", null ],
      [ "user_interface.c", "user__interface_8c.html", null ],
      [ "user_interface.h", "user__interface_8h.html", null ],
      [ "util.c", "util_8c.html", null ],
      [ "util.h", "util_8h.html", null ],
      [ "utilMisc.c", "util_misc_8c.html", null ],
      [ "utilMisc.h", "util_misc_8h.html", null ]
    ] ],
    [ "Globals", "globals.html", null ]
  ] ]
];

function createIndent(o,domNode,node,level)
{
  if (node.parentNode && node.parentNode.parentNode)
  {
    createIndent(o,domNode,node.parentNode,level+1);
  }
  var imgNode = document.createElement("img");
  if (level==0 && node.childrenData)
  {
    node.plus_img = imgNode;
    node.expandToggle = document.createElement("a");
    node.expandToggle.href = "javascript:void(0)";
    node.expandToggle.onclick = function() 
    {
      if (node.expanded) 
      {
        $(node.getChildrenUL()).slideUp("fast");
        if (node.isLast)
        {
          node.plus_img.src = node.relpath+"ftv2plastnode.png";
        }
        else
        {
          node.plus_img.src = node.relpath+"ftv2pnode.png";
        }
        node.expanded = false;
      } 
      else 
      {
        expandNode(o, node, false);
      }
    }
    node.expandToggle.appendChild(imgNode);
    domNode.appendChild(node.expandToggle);
  }
  else
  {
    domNode.appendChild(imgNode);
  }
  if (level==0)
  {
    if (node.isLast)
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2plastnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2lastnode.png";
        domNode.appendChild(imgNode);
      }
    }
    else
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2pnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2node.png";
        domNode.appendChild(imgNode);
      }
    }
  }
  else
  {
    if (node.isLast)
    {
      imgNode.src = node.relpath+"ftv2blank.png";
    }
    else
    {
      imgNode.src = node.relpath+"ftv2vertline.png";
    }
  }
  imgNode.border = "0";
}

function newNode(o, po, text, link, childrenData, lastNode)
{
  var node = new Object();
  node.children = Array();
  node.childrenData = childrenData;
  node.depth = po.depth + 1;
  node.relpath = po.relpath;
  node.isLast = lastNode;

  node.li = document.createElement("li");
  po.getChildrenUL().appendChild(node.li);
  node.parentNode = po;

  node.itemDiv = document.createElement("div");
  node.itemDiv.className = "item";

  node.labelSpan = document.createElement("span");
  node.labelSpan.className = "label";

  createIndent(o,node.itemDiv,node,0);
  node.itemDiv.appendChild(node.labelSpan);
  node.li.appendChild(node.itemDiv);

  var a = document.createElement("a");
  node.labelSpan.appendChild(a);
  node.label = document.createTextNode(text);
  a.appendChild(node.label);
  if (link) 
  {
    a.href = node.relpath+link;
  } 
  else 
  {
    if (childrenData != null) 
    {
      a.className = "nolink";
      a.href = "javascript:void(0)";
      a.onclick = node.expandToggle.onclick;
      node.expanded = false;
    }
  }

  node.childrenUL = null;
  node.getChildrenUL = function() 
  {
    if (!node.childrenUL) 
    {
      node.childrenUL = document.createElement("ul");
      node.childrenUL.className = "children_ul";
      node.childrenUL.style.display = "none";
      node.li.appendChild(node.childrenUL);
    }
    return node.childrenUL;
  };

  return node;
}

function showRoot()
{
  var headerHeight = $("#top").height();
  var footerHeight = $("#nav-path").height();
  var windowHeight = $(window).height() - headerHeight - footerHeight;
  navtree.scrollTo('#selected',0,{offset:-windowHeight/2});
}

function expandNode(o, node, imm)
{
  if (node.childrenData && !node.expanded) 
  {
    if (!node.childrenVisited) 
    {
      getNode(o, node);
    }
    if (imm)
    {
      $(node.getChildrenUL()).show();
    } 
    else 
    {
      $(node.getChildrenUL()).slideDown("fast",showRoot);
    }
    if (node.isLast)
    {
      node.plus_img.src = node.relpath+"ftv2mlastnode.png";
    }
    else
    {
      node.plus_img.src = node.relpath+"ftv2mnode.png";
    }
    node.expanded = true;
  }
}

function getNode(o, po)
{
  po.childrenVisited = true;
  var l = po.childrenData.length-1;
  for (var i in po.childrenData) 
  {
    var nodeData = po.childrenData[i];
    po.children[i] = newNode(o, po, nodeData[0], nodeData[1], nodeData[2],
        i==l);
  }
}

function findNavTreePage(url, data)
{
  var nodes = data;
  var result = null;
  for (var i in nodes) 
  {
    var d = nodes[i];
    if (d[1] == url) 
    {
      return new Array(i);
    }
    else if (d[2] != null) // array of children
    {
      result = findNavTreePage(url, d[2]);
      if (result != null) 
      {
        return (new Array(i).concat(result));
      }
    }
  }
  return null;
}

function initNavTree(toroot,relpath)
{
  var o = new Object();
  o.toroot = toroot;
  o.node = new Object();
  o.node.li = document.getElementById("nav-tree-contents");
  o.node.childrenData = NAVTREE;
  o.node.children = new Array();
  o.node.childrenUL = document.createElement("ul");
  o.node.getChildrenUL = function() { return o.node.childrenUL; };
  o.node.li.appendChild(o.node.childrenUL);
  o.node.depth = 0;
  o.node.relpath = relpath;

  getNode(o, o.node);

  o.breadcrumbs = findNavTreePage(toroot, NAVTREE);
  if (o.breadcrumbs == null)
  {
    o.breadcrumbs = findNavTreePage("index.html",NAVTREE);
  }
  if (o.breadcrumbs != null && o.breadcrumbs.length>0)
  {
    var p = o.node;
    for (var i in o.breadcrumbs) 
    {
      var j = o.breadcrumbs[i];
      p = p.children[j];
      expandNode(o,p,true);
    }
    p.itemDiv.className = p.itemDiv.className + " selected";
    p.itemDiv.id = "selected";
    $(window).load(showRoot);
  }
}

