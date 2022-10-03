<template>
  <div id="terminal" ref="terminal"></div>
</template>

<script>
import "xterm/css/xterm.css";
import qwc from "./common/js/qwebchannel.js";
import { Terminal } from "xterm";
import { FitAddon } from "xterm-addon-fit";
// import { SearchAddon } from 'xterm-addon-search';
// import { WebLinksAddon } from 'xterm-addon-web-links';
export default {
  name: "App",
  components: {
    // HelloWorld
  },
  data() {
    return {
      xterm: null,
      fitAddon: null,
      rows: 40,
      cols: 100,
      fontSize:16,
      clientId: null,
    };
  },
  mounted() {
    window.setClientId=this.setClientId
    window.xtermWrite=this.xtermWrite
    window.connectSuccess=this.connectSuccess
    
    new qwc.QWebChannel(qt.webChannelTransport, function (channel) {
      window.core = channel.objects.core;
    });
    
    const terminal = new Terminal({
      termName: "xterm",
      useStyle: true,
      convertEol: true,
      screenKeys: true,
      cursorBlink: true,
      visualBell: true,
      colors: Terminal.xtermColors,
      cursorStyle:"block",
      fontSize:this.fontSize,
      scrollback: 200, //终端中的回滚量
      rendererType: "dom",
      // rendererType: "canvas",
      // lineHeight: 1,
      // cols: 80,
      // rows: 24,
      theme: {
        // foreground: "#202935", //字体
        background: "#202935", //背景色
        cursorAccent:"#202935",
        // cursor: "help", //设置光标
      }
    });
    this.xterm = terminal;
    terminal.open(this.$refs["terminal"]);
    this.fitAddon = new FitAddon();
    terminal.loadAddon(this.fitAddon);
    this.fitAddon.fit();
    terminal.focus();

    let rc = this.getRowsAndCols();
    terminal.resize(rc.cols, rc.rows);

    window.addEventListener("resize",this.onResize);

    if (terminal._initialized) return;
    // 初始化
    terminal._initialized = true;
    terminal.onData((key) => {
      window.core.recieveJsMessage(key);
    });
  },
  methods: {
    onResize: function () {
      let rc = this.getRowsAndCols();
      this.xterm.resize(rc.cols, rc.rows);
      this.requestPtySize();
    },
    getRowsAndCols: function () {
      this.fitAddon.fit();
      // this.xterm.scrollToBottom();
      var height = document.getElementById("terminal").clientHeight
      var lineHeight = height/this.xterm.rows
      var rows = parseInt(document.documentElement.clientHeight/lineHeight)-1
      return {
        rows: rows,
        cols: this.xterm.cols,
      };
    },
    requestPtySize: function () {
      let rc = this.getRowsAndCols();
      var ss = rc.rows + "," + rc.cols;
      if (window.core != undefined) {
        window.core.setChannelRequestPtySize(ss);
      }
    },

    setClientId: function (cId) {
      this.clientId = cId;
      this.open_websocket();
      setTimeout(()=>{
        window.core.ssh2connect("");
      },2000)
    },

    xtermWrite:function(data){
      this.xterm.write(data.data);
    },

    connectSuccess:function(){
      this.requestPtySize()
    },

    open_websocket: function () {
      if (location.search != "")
        var baseUrl = /[?&]webChannelBaseUrl=([A-Za-z0-9\-:/\.]+)/.exec(
          location.search
        )[1];
      else var baseUrl = "ws://localhost:12345";

      var socket = new WebSocket(baseUrl);

      socket.onclose = function () {
        console.error("web channel closed");
      };
      socket.onerror = function (error) {
        console.error("web channel error: " + error);
      };
      socket.onopen = ()=>{
        socket.send(this.clientId);
      };
      socket.onmessage = (evt)=>{
        var received_msg = evt.data;
        this.xterm.write(received_msg);
      };
    },
  },
};
</script>

<style>
body{
  margin: 0;
  padding: 0;
}
#terminal{
  margin: 0;
  padding: 0;
}
::-webkit-scrollbar {
  /*滚动条整体样式*/
  width: 6px; /*高宽分别对应横竖滚动条的尺寸*/
  height: 1px;
}
::-webkit-scrollbar-thumb {
  /*滚动条里面小方块*/
  border-radius: 10px;
  box-shadow: inset 0 0 5px rgba(97, 184, 179, 0.1);
  background: #202935;
}
::-webkit-scrollbar-track {
  /*滚动条里面轨道*/
  box-shadow: inset 0 0 5px rgba(87, 175, 187, 0.1);
  border-radius: 10px;
  background: #ededed;
}
</style>
