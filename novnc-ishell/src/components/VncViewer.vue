<template>
    <div class="page-home" ref="canvas">
        <div id="top_bar">
            <div id="status">Loading</div>
            <div id="sendCtrlAltDelButton" @click="sendCtrlAltDel">Send CtrlAltDel</div>
        </div>
        <div id="screen">
        </div>
    </div>
</template>

<script>
import RFB from '@novnc/novnc/core/rfb'
import qwc from '../common/js/qwebchannel.js'
export default {
    name: 'novnc',
    data() {
        return {
            rfb: null,
            title: '',
            password: '123456',
            desktopName: '',
            clietnId: '',
            isFullscreen: false,
            dialogVisible: true,
            webSocketServerPort: 0,
        }
    },
    mounted() {
        window.setClientId = this.setClientId;
        window.setPassword = this.setPassword;
        window.setWebSocketServerPort = this.setWebSocketServerPort;
        new qwc.QWebChannel(qt.webChannelTransport, function (channel) {
            window.core = channel.objects.core;
        });
    },

    // 销毁 断开连接
    beforeDestroy() {
        console.log('销毁 断开连接')
        this.rfb && this.rfb._disconnect()
    },

    methods: {
        setWebSocketServerPort(port) {
            this.webSocketServerPort = port;
        },
        setPassword(password) {
            this.password = password;
        },
        setClientId(clietnId) {
            this.clietnId = clietnId;
            this.connectVnc();
        },
        handleClose() {
            this.$emit('close')
        },
        sendCtrlAltDel() {
            this.rfb.sendCtrlAltDel()
            return false
        },
        updateDesktopName(e) {
            this.desktopName = e.detail.name
        },
        credentialsAreRequired(e) {
            const password = prompt("请输入密码:");
            this.rfb.sendCredentials({ password: password });
        },
        // 连接
        connectVnc() {
            const url = 'ws://127.0.0.1:' + this.webSocketServerPort + '/vncviewer?clientId=' + this.clietnId;
            // const url = 'ws://127.0.0.1:5900/vncviewer?target=192.168.0.106:5900';
            console.error("url ==> " + url);
            this.rfb = new RFB(document.getElementById('screen'), url, {
                credentials: { password: this.password }
            })
            this.rfb.addEventListener('connect', this.connectedToServer)
            this.rfb.addEventListener('disconnect', this.disconnectedFromServer)
            this.rfb.addEventListener('desktopname', this.updateDesktopName)
            this.rfb.addEventListener("credentialsrequired", this.credentialsAreRequired);
            this.rfb.scaleViewport = false // scaleViewport指示是否应在本地扩展远程会话以使其适合其容器。禁用时，如果远程会话小于其容器，则它将居中，或者根据clipViewport它是否更大来处理。默认情况下禁用。
            this.rfb.viewOnly = false
            // console.log(this.rfb)
        },

        status(text) {
            document.getElementById('status').textContent = text
        },

        connectedToServer(e) {
            this.status('Connected to ' + this.desktopName)
            console.log('success', e)
        },

        disconnectedFromServer(e) {
            if (e.detail.clean) {
                this.status('Disconnected')

                console.log('clean', e.detail.clean)
                // 根据 断开信息的msg.detail.clean 来判断是否可以重新连接
                // this.connectVnc()
            } else {
                this.status('Something went wrong, connection is closed')
                console.log('链接失败,重新链接中-------' + this.wsURL)
                // this.connectVnc()
            }
        }
    }
}
</script>

<style scoped>
.page-home {
    width: 100%;
    height: 100%;
    margin: 0;
    padding: 0;
    background-color: dimgrey;
    display: flex;
    flex-direction: column;
}

#top_bar {
    background-color: #6e84a3;
    color: white;
    font: bold 12px Helvetica;
    padding: 6px 5px 4px 5px;
    border-bottom: 1px outset;
}

#status {
    text-align: center;
}

#sendCtrlAltDelButton {
    position: absolute;
    top: 0px;
    right: 0px;
    border: 1px outset;
    padding: 5px 5px 4px 5px;
    cursor: pointer;
}

#screen {
    flex: 1;
    /* fill remaining space */
    overflow: hidden;
}
</style>