<template>
  <div id="app">
    <!-- <img alt="Vue logo" src="./assets/logo.png">
    <HelloWorld msg="Welcome to Your Vue.js App"/> -->
    <div id="terminal" ref="terminal"></div>
  </div>
</template>

<script>
// import HelloWorld from './components/HelloWorld.vue'
import "xterm/css/xterm.css";
import qwc from "./common/js/qwebchannel.js";
import { Terminal } from "xterm";
import { FitAddon } from "xterm-addon-fit";
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
      clientId: null,
    };
  },
  created(){
    window.setClientId=this.setClientId
  },
  mounted() {
    window.setClientId=this.setClientId
    
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
      rendererType: "dom",
    });
    this.xterm = terminal;
    this.fitAddon = new FitAddon();
    terminal.loadAddon(this.fitAddon);
    terminal.open(this.$refs["terminal"]);
    this.fitAddon.fit();

    // terminal.writeln("hello world")

    terminal.focus();
    terminal.prompt = () => {
      terminal.write("\r\n$ ");
    };

    let rc = this.getRowsAndCols();
    terminal.resize(rc.cols, rc.rows);

    window.addEventListener("resize", () => {
      this.fitAddon.fit();
      let rc = this.getRowsAndCols();
      terminal.resize(rc.cols, rc.rows);
      this.requestPtySize();
    });

    if (terminal._initialized) return;
    // 初始化
    terminal._initialized = true;
    // terminal.prompt();
    // 添加事件监听器，支持输入方法
    // terminal.onKey((e) => {
    //   const printable =
    //     !e.domEvent.altKey &&
    //     !e.domEvent.altGraphKey &&
    //     !e.domEvent.ctrlKey &&
    //     !e.domEvent.metaKey;
    //   if (e.domEvent.keyCode === 13) {
    //     terminal.prompt();
    //   } else if (e.domEvent.keyCode === 8) {
    //     // back 删除的情况
    //     if (terminal._core.buffer.x > 2) {
    //       terminal.write("\b \b");
    //     }
    //   } else if (printable) {
    //     // terminal.write(e.key);
    //   }
    //   console.log(1, "print", e.key);
    // });
    terminal.onData((key) => {
      // terminal.write(key);
      window.core.recieveJsMessage(key);
    });
  },
  methods: {
    getRowsAndCols: function () {
      this.xterm.scrollToBottom();
      let height = document.documentElement.clientHeight;
      let rows = parseInt(height / 17)-1;

      var span = $("<span>", {
        text: "qwertyuiopasdfghjklzxcvbnm",
      });
      $("#terminal").append(span);
      var span_width = span.width();
      // var span_height = span.height();
      span.remove();

      let width = document.body.clientWidth;
      let cols = parseInt(width / (span_width/26));
      return {
        rows: rows,
        cols: cols,
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
  overflow: hidden;
}
#app {
  /* font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px; */
  /* margin: 2px; */
  /* padding: 2px; */
}
</style>
