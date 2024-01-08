<template>
  <div id="app">
    <VncViewer />
  </div>
</template>

<script>
import VncViewer from './components/VncViewer.vue'

export default {
  name: 'App',
  components: {
    VncViewer
  }
}
</script>

<style>
body {
  margin: 0;
  background-color: #202935;
  height: 100%;
  display: flex;
  flex-direction: column;
}

html {
  height: 100%;
}

#top_bar {
  background-color: red;
  color: white;
  font: bold 12px Helvetica;
  padding: 6px 5px 4px 5px;
  border-bottom: 1px outset;
}

#status {
  text-align: center;
}

#sendCtrlAltDelButton {
  position: fixed;
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
  background-color: #202935;
}

#screen div {
  background-color: #202935 !important;
}
</style>
