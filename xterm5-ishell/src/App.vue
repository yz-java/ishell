<template>
  <div id="terminal" ref="terminal"></div>
</template>

<script>
import qwc from "./common/js/qwebchannel.js";
import "xterm/css/xterm.css";
import { Terminal } from "xterm";
import { FitAddon } from "xterm-addon-fit";
import { CanvasAddon } from 'xterm-addon-canvas';
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
      fontSize: 18,
      clientId: null,
      wsClient: null,
      webSocketServerPort: 0,
    };
  },
  mounted() {
    window.setClientId = this.setClientId
    window.xtermWrite = this.xtermWrite
    window.connectSuccess = this.connectSuccess
    window.closews = this.closews
    window.setWebSocketServerPort = this.setWebSocketServerPort;
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
      cursorStyle: "block",
      fontSize: this.fontSize,
      scrollback: 500, //终端中的回滚量
      // rendererType: "dom",
      rendererType: "canvas",
      // lineHeight: 1,
      // cols: 80,
      // rows: 24,
      theme: {
        // foreground: "#202935", //字体
        background: "#202935", //背景色
        cursorAccent: "#202935",
        // cursor: "help", //设置光标
      }
    });
    this.xterm = terminal;
    const fitAddon = new FitAddon();
    this.fitAddon = fitAddon;
    terminal.loadAddon(fitAddon);
    terminal.open(this.$refs["terminal"]);
    // terminal.loadAddon(new WebglAddon());
    terminal.loadAddon(new CanvasAddon());
    fitAddon.fit(); //黑窗口适应实际div宽高
    terminal.focus();

    window.addEventListener("resize", this.onResize);

    terminal.onData((key) => {
      window.core.recieveJsMessage(key);
    });
    // terminal.onKey(e => {
    //   window.core.recieveJsMessage(e.key);
    // });
    terminal.onResize((size) => {
      console.error("terminal onResize cols  " + size.cols + " rows " + size.rows)
      var ss = size.rows + "," + size.cols;
      if (window.core != undefined) {
        window.core.setChannelRequestPtySize(ss);
      }
    });
  },
  methods: {
    setWebSocketServerPort(port) {
      this.webSocketServerPort = port;
    },
    onResize: function () {
      this.fitAddon.fit();
      let rc = this.getRowsAndCols();
      console.error("window onResize cols  " + rc.cols + " rows " + rc.rows)
      this.xterm.resize(rc.cols, rc.rows);
    },
    getRowsAndCols: function () {
      var height = document.getElementById("terminal").clientHeight
      var lineHeight = height / this.xterm.rows
      var rows = parseInt(document.documentElement.clientHeight / lineHeight)
      return {
        rows: rows,
        cols: this.xterm.cols,
      };
    },

    setClientId: function (cId) {
      this.clientId = cId;
      this.open_websocket();
      // setTimeout(()=>{
      //   window.core.ssh2connect("");
      // },2000)
    },

    xtermWrite: function (data) {
      this.xterm.write(data.data);
    },

    connectSuccess: function () {
      let rc = this.getRowsAndCols();
      this.xterm.resize(rc.cols, rc.rows);
      if (window.core != undefined) {
        var ss = rc.rows + "," + rc.cols;
        console.error("设置pty " + ss);
        window.core.setChannelRequestPtySize(ss);
      }
    },
    closews: function () {
      this.wsClient.close()
    },

    open_websocket: function () {
      var baseUrl = "ws://localhost:" + this.webSocketServerPort + "?clientId=" + this.clientId;
      var socket = new WebSocket(baseUrl);
      this.wsClient = socket;
      socket.onclose = function () {
        console.error("web channel closed");
      };
      socket.onerror = function (error) {
        console.error("web channel error: " + error);
      };
      socket.onopen = () => {
        socket.send(this.clientId);
      };
      socket.onmessage = (evt) => {
        var received_msg = evt.data;
        this.xterm.write(received_msg);
      };
    },
  },
};
</script>

<style>
body {
  margin: 0;
  padding: 0;
  background-color: #202935;
}

#terminal {
  margin: 0;
  padding: 0;
}

::-webkit-scrollbar {
  /*滚动条整体样式*/
  width: 6px;
  /*高宽分别对应横竖滚动条的尺寸*/
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
