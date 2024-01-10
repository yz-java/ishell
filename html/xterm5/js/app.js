/*
 * ATTENTION: The "eval" devtool has been used (maybe by default in mode: "development").
 * This devtool is neither made for production nor for readable output files.
 * It uses "eval()" calls to create a separate source file in the browser devtools.
 * If you are trying to read the output file, select a different devtool (https://webpack.js.org/configuration/devtool/)
 * or disable the default devtool with "devtool: false".
 * If you are looking for production-ready output files, see mode: "production" (https://webpack.js.org/configuration/mode/).
 */
/******/ (function() { // webpackBootstrap
/******/ 	var __webpack_modules__ = ({

/***/ "./node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use[0]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=script&lang=js":
/*!**************************************************************************************************************************************************************************!*\
  !*** ./node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use[0]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=script&lang=js ***!
  \**************************************************************************************************************************************************************************/
/***/ (function(__unused_webpack_module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _common_js_qwebchannel_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ./common/js/qwebchannel.js */ \"./src/common/js/qwebchannel.js\");\n/* harmony import */ var _common_js_qwebchannel_js__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(_common_js_qwebchannel_js__WEBPACK_IMPORTED_MODULE_0__);\n/* harmony import */ var xterm_css_xterm_css__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! xterm/css/xterm.css */ \"./node_modules/xterm/css/xterm.css\");\n/* harmony import */ var xterm_css_xterm_css__WEBPACK_IMPORTED_MODULE_1___default = /*#__PURE__*/__webpack_require__.n(xterm_css_xterm_css__WEBPACK_IMPORTED_MODULE_1__);\n/* harmony import */ var xterm__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! xterm */ \"./node_modules/xterm/lib/xterm.js\");\n/* harmony import */ var xterm__WEBPACK_IMPORTED_MODULE_2___default = /*#__PURE__*/__webpack_require__.n(xterm__WEBPACK_IMPORTED_MODULE_2__);\n/* harmony import */ var xterm_addon_fit__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! xterm-addon-fit */ \"./node_modules/.store/xterm-addon-fit@0.6.0/node_modules/xterm-addon-fit/lib/xterm-addon-fit.js\");\n/* harmony import */ var xterm_addon_fit__WEBPACK_IMPORTED_MODULE_3___default = /*#__PURE__*/__webpack_require__.n(xterm_addon_fit__WEBPACK_IMPORTED_MODULE_3__);\n/* harmony import */ var xterm_addon_canvas__WEBPACK_IMPORTED_MODULE_4__ = __webpack_require__(/*! xterm-addon-canvas */ \"./node_modules/.store/xterm-addon-canvas@0.2.0/node_modules/xterm-addon-canvas/lib/xterm-addon-canvas.js\");\n/* harmony import */ var xterm_addon_canvas__WEBPACK_IMPORTED_MODULE_4___default = /*#__PURE__*/__webpack_require__.n(xterm_addon_canvas__WEBPACK_IMPORTED_MODULE_4__);\n\n\n\n\n\n/* harmony default export */ __webpack_exports__[\"default\"] = ({\n  name: \"App\",\n  components: {\n    // HelloWorld\n  },\n  data() {\n    return {\n      xterm: null,\n      fitAddon: null,\n      rows: 40,\n      cols: 100,\n      fontSize: 18,\n      clientId: null,\n      wsClient: null,\n      webSocketServerPort: 0\n    };\n  },\n  mounted() {\n    window.setClientId = this.setClientId;\n    window.xtermWrite = this.xtermWrite;\n    window.connectSuccess = this.connectSuccess;\n    window.closews = this.closews;\n    window.setWebSocketServerPort = this.setWebSocketServerPort;\n    new (_common_js_qwebchannel_js__WEBPACK_IMPORTED_MODULE_0___default().QWebChannel)(qt.webChannelTransport, function (channel) {\n      window.core = channel.objects.core;\n    });\n    const terminal = new xterm__WEBPACK_IMPORTED_MODULE_2__.Terminal({\n      termName: \"xterm\",\n      useStyle: true,\n      convertEol: true,\n      screenKeys: true,\n      cursorBlink: true,\n      visualBell: true,\n      colors: xterm__WEBPACK_IMPORTED_MODULE_2__.Terminal.xtermColors,\n      cursorStyle: \"block\",\n      fontSize: this.fontSize,\n      scrollback: 500,\n      //终端中的回滚量\n      // rendererType: \"dom\",\n      rendererType: \"canvas\",\n      // lineHeight: 1,\n      // cols: 80,\n      // rows: 24,\n      theme: {\n        // foreground: \"#202935\", //字体\n        background: \"#202935\",\n        //背景色\n        cursorAccent: \"#202935\"\n        // cursor: \"help\", //设置光标\n      }\n    });\n    this.xterm = terminal;\n    const fitAddon = new xterm_addon_fit__WEBPACK_IMPORTED_MODULE_3__.FitAddon();\n    this.fitAddon = fitAddon;\n    terminal.loadAddon(fitAddon);\n    terminal.open(this.$refs[\"terminal\"]);\n    // terminal.loadAddon(new WebglAddon());\n    terminal.loadAddon(new xterm_addon_canvas__WEBPACK_IMPORTED_MODULE_4__.CanvasAddon());\n    fitAddon.fit(); //黑窗口适应实际div宽高\n    terminal.focus();\n    window.addEventListener(\"resize\", this.onResize);\n    terminal.onData(key => {\n      window.core.recieveJsMessage(key);\n    });\n    // terminal.onKey(e => {\n    //   window.core.recieveJsMessage(e.key);\n    // });\n    terminal.onResize(size => {\n      console.error(\"terminal onResize cols  \" + size.cols + \" rows \" + size.rows);\n      var ss = size.rows + \",\" + size.cols;\n      if (window.core != undefined) {\n        window.core.setChannelRequestPtySize(ss);\n      }\n    });\n  },\n  methods: {\n    setWebSocketServerPort(port) {\n      this.webSocketServerPort = port;\n    },\n    onResize: function () {\n      this.fitAddon.fit();\n      let rc = this.getRowsAndCols();\n      console.error(\"window onResize cols  \" + rc.cols + \" rows \" + rc.rows);\n      this.xterm.resize(rc.cols, rc.rows);\n    },\n    getRowsAndCols: function () {\n      var height = document.getElementById(\"terminal\").clientHeight;\n      var lineHeight = height / this.xterm.rows;\n      var rows = parseInt(document.documentElement.clientHeight / lineHeight);\n      return {\n        rows: rows,\n        cols: this.xterm.cols\n      };\n    },\n    setClientId: function (cId) {\n      this.clientId = cId;\n      this.open_websocket();\n      // setTimeout(()=>{\n      //   window.core.ssh2connect(\"\");\n      // },2000)\n    },\n    xtermWrite: function (data) {\n      this.xterm.write(data.data);\n    },\n    connectSuccess: function () {\n      let rc = this.getRowsAndCols();\n      this.xterm.resize(rc.cols, rc.rows);\n      if (window.core != undefined) {\n        var ss = rc.rows + \",\" + rc.cols;\n        console.error(\"设置pty \" + ss);\n        window.core.setChannelRequestPtySize(ss);\n      }\n    },\n    closews: function () {\n      this.wsClient.close();\n    },\n    open_websocket: function () {\n      var baseUrl = \"ws://localhost:\" + this.webSocketServerPort + \"?clientId=\" + this.clientId;\n      var socket = new WebSocket(baseUrl);\n      this.wsClient = socket;\n      socket.onclose = function () {\n        console.error(\"web channel closed\");\n      };\n      socket.onerror = function (error) {\n        console.error(\"web channel error: \" + error);\n      };\n      socket.onopen = () => {\n        socket.send(this.clientId);\n      };\n      socket.onmessage = evt => {\n        var received_msg = evt.data;\n        this.xterm.write(received_msg);\n      };\n    }\n  }\n});\n\n//# sourceURL=webpack://xterm5-ishell/./src/App.vue?./node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use%5B0%5D!./node_modules/vue-loader/dist/index.js??ruleSet%5B0%5D.use%5B0%5D");

/***/ }),

/***/ "./node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use[0]!./node_modules/vue-loader/dist/templateLoader.js??ruleSet[1].rules[3]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=template&id=7ba5bd90":
/*!******************************************************************************************************************************************************************************************************************************************************!*\
  !*** ./node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use[0]!./node_modules/vue-loader/dist/templateLoader.js??ruleSet[1].rules[3]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=template&id=7ba5bd90 ***!
  \******************************************************************************************************************************************************************************************************************************************************/
/***/ (function(__unused_webpack_module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony export */ __webpack_require__.d(__webpack_exports__, {\n/* harmony export */   render: function() { return /* binding */ render; }\n/* harmony export */ });\n/* harmony import */ var vue__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! vue */ \"./node_modules/vue/dist/vue.runtime.esm-bundler.js\");\n\nconst _hoisted_1 = {\n  id: \"terminal\",\n  ref: \"terminal\"\n};\nfunction render(_ctx, _cache, $props, $setup, $data, $options) {\n  return (0,vue__WEBPACK_IMPORTED_MODULE_0__.openBlock)(), (0,vue__WEBPACK_IMPORTED_MODULE_0__.createElementBlock)(\"div\", _hoisted_1, null, 512 /* NEED_PATCH */);\n}\n\n//# sourceURL=webpack://xterm5-ishell/./src/App.vue?./node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use%5B0%5D!./node_modules/vue-loader/dist/templateLoader.js??ruleSet%5B1%5D.rules%5B3%5D!./node_modules/vue-loader/dist/index.js??ruleSet%5B0%5D.use%5B0%5D");

/***/ }),

/***/ "./src/common/js/qwebchannel.js":
/*!**************************************!*\
  !*** ./src/common/js/qwebchannel.js ***!
  \**************************************/
/***/ (function(module, __unused_webpack_exports, __webpack_require__) {

"use strict";
eval("/****************************************************************************\r\n**\r\n** Copyright (C) 2016 The Qt Company Ltd.\r\n** Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Milian Wolff <milian.wolff@kdab.com>\r\n** Contact: https://www.qt.io/licensing/\r\n**\r\n** This file is part of the QtWebChannel module of the Qt Toolkit.\r\n**\r\n** $QT_BEGIN_LICENSE:LGPL$\r\n** Commercial License Usage\r\n** Licensees holding valid commercial Qt licenses may use this file in\r\n** accordance with the commercial license agreement provided with the\r\n** Software or, alternatively, in accordance with the terms contained in\r\n** a written agreement between you and The Qt Company. For licensing terms\r\n** and conditions see https://www.qt.io/terms-conditions. For further\r\n** information use the contact form at https://www.qt.io/contact-us.\r\n**\r\n** GNU Lesser General Public License Usage\r\n** Alternatively, this file may be used under the terms of the GNU Lesser\r\n** General Public License version 3 as published by the Free Software\r\n** Foundation and appearing in the file LICENSE.LGPL3 included in the\r\n** packaging of this file. Please review the following information to\r\n** ensure the GNU Lesser General Public License version 3 requirements\r\n** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.\r\n**\r\n** GNU General Public License Usage\r\n** Alternatively, this file may be used under the terms of the GNU\r\n** General Public License version 2.0 or (at your option) the GNU General\r\n** Public license version 3 or any later version approved by the KDE Free\r\n** Qt Foundation. The licenses are as published by the Free Software\r\n** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3\r\n** included in the packaging of this file. Please review the following\r\n** information to ensure the GNU General Public License requirements will\r\n** be met: https://www.gnu.org/licenses/gpl-2.0.html and\r\n** https://www.gnu.org/licenses/gpl-3.0.html.\r\n**\r\n** $QT_END_LICENSE$\r\n**\r\n****************************************************************************/\n\n\n\n__webpack_require__(/*! core-js/modules/es.array.push.js */ \"./node_modules/core-js/modules/es.array.push.js\");\nvar QWebChannelMessageTypes = {\n  signal: 1,\n  propertyUpdate: 2,\n  init: 3,\n  idle: 4,\n  debug: 5,\n  invokeMethod: 6,\n  connectToSignal: 7,\n  disconnectFromSignal: 8,\n  setProperty: 9,\n  response: 10\n};\nvar QWebChannel = function (transport, initCallback) {\n  if (typeof transport !== \"object\" || typeof transport.send !== \"function\") {\n    console.error(\"The QWebChannel expects a transport object with a send function and onmessage callback property.\" + \" Given is: transport: \" + typeof transport + \", transport.send: \" + typeof transport.send);\n    return;\n  }\n  var channel = this;\n  this.transport = transport;\n  this.send = function (data) {\n    if (typeof data !== \"string\") {\n      data = JSON.stringify(data);\n    }\n    channel.transport.send(data);\n  };\n  this.transport.onmessage = function (message) {\n    var data = message.data;\n    if (typeof data === \"string\") {\n      data = JSON.parse(data);\n    }\n    switch (data.type) {\n      case QWebChannelMessageTypes.signal:\n        channel.handleSignal(data);\n        break;\n      case QWebChannelMessageTypes.response:\n        channel.handleResponse(data);\n        break;\n      case QWebChannelMessageTypes.propertyUpdate:\n        channel.handlePropertyUpdate(data);\n        break;\n      default:\n        console.error(\"invalid message received:\", message.data);\n        break;\n    }\n  };\n  this.execCallbacks = {};\n  this.execId = 0;\n  this.exec = function (data, callback) {\n    if (!callback) {\n      // if no callback is given, send directly\n      channel.send(data);\n      return;\n    }\n    if (channel.execId === Number.MAX_VALUE) {\n      // wrap\n      channel.execId = Number.MIN_VALUE;\n    }\n    if (data.hasOwnProperty(\"id\")) {\n      console.error(\"Cannot exec message with property id: \" + JSON.stringify(data));\n      return;\n    }\n    data.id = channel.execId++;\n    channel.execCallbacks[data.id] = callback;\n    channel.send(data);\n  };\n  this.objects = {};\n  this.handleSignal = function (message) {\n    var object = channel.objects[message.object];\n    if (object) {\n      object.signalEmitted(message.signal, message.args);\n    } else {\n      console.warn(\"Unhandled signal: \" + message.object + \"::\" + message.signal);\n    }\n  };\n  this.handleResponse = function (message) {\n    if (!message.hasOwnProperty(\"id\")) {\n      console.error(\"Invalid response message received: \", JSON.stringify(message));\n      return;\n    }\n    channel.execCallbacks[message.id](message.data);\n    delete channel.execCallbacks[message.id];\n  };\n  this.handlePropertyUpdate = function (message) {\n    for (var i in message.data) {\n      var data = message.data[i];\n      var object = channel.objects[data.object];\n      if (object) {\n        object.propertyUpdate(data.signals, data.properties);\n      } else {\n        console.warn(\"Unhandled property update: \" + data.object + \"::\" + data.signal);\n      }\n    }\n    channel.exec({\n      type: QWebChannelMessageTypes.idle\n    });\n  };\n  this.debug = function (message) {\n    channel.send({\n      type: QWebChannelMessageTypes.debug,\n      data: message\n    });\n  };\n  channel.exec({\n    type: QWebChannelMessageTypes.init\n  }, function (data) {\n    for (var objectName in data) {\n      var object = new QObject(objectName, data[objectName], channel);\n    }\n    // now unwrap properties, which might reference other registered objects\n    for (var objectName in channel.objects) {\n      channel.objects[objectName].unwrapProperties();\n    }\n    if (initCallback) {\n      initCallback(channel);\n    }\n    channel.exec({\n      type: QWebChannelMessageTypes.idle\n    });\n  });\n};\nfunction QObject(name, data, webChannel) {\n  this.__id__ = name;\n  webChannel.objects[name] = this;\n\n  // List of callbacks that get invoked upon signal emission\n  this.__objectSignals__ = {};\n\n  // Cache of all properties, updated when a notify signal is emitted\n  this.__propertyCache__ = {};\n  var object = this;\n\n  // ----------------------------------------------------------------------\n\n  this.unwrapQObject = function (response) {\n    if (response instanceof Array) {\n      // support list of objects\n      var ret = new Array(response.length);\n      for (var i = 0; i < response.length; ++i) {\n        ret[i] = object.unwrapQObject(response[i]);\n      }\n      return ret;\n    }\n    if (!response || !response[\"__QObject*__\"] || response.id === undefined) {\n      return response;\n    }\n    var objectId = response.id;\n    if (webChannel.objects[objectId]) return webChannel.objects[objectId];\n    if (!response.data) {\n      console.error(\"Cannot unwrap unknown QObject \" + objectId + \" without data.\");\n      return;\n    }\n    var qObject = new QObject(objectId, response.data, webChannel);\n    qObject.destroyed.connect(function () {\n      if (webChannel.objects[objectId] === qObject) {\n        delete webChannel.objects[objectId];\n        // reset the now deleted QObject to an empty {} object\n        // just assigning {} though would not have the desired effect, but the\n        // below also ensures all external references will see the empty map\n        // NOTE: this detour is necessary to workaround QTBUG-40021\n        var propertyNames = [];\n        for (var propertyName in qObject) {\n          propertyNames.push(propertyName);\n        }\n        for (var idx in propertyNames) {\n          delete qObject[propertyNames[idx]];\n        }\n      }\n    });\n    // here we are already initialized, and thus must directly unwrap the properties\n    qObject.unwrapProperties();\n    return qObject;\n  };\n  this.unwrapProperties = function () {\n    for (var propertyIdx in object.__propertyCache__) {\n      object.__propertyCache__[propertyIdx] = object.unwrapQObject(object.__propertyCache__[propertyIdx]);\n    }\n  };\n  function addSignal(signalData, isPropertyNotifySignal) {\n    var signalName = signalData[0];\n    var signalIndex = signalData[1];\n    object[signalName] = {\n      connect: function (callback) {\n        if (typeof callback !== \"function\") {\n          console.error(\"Bad callback given to connect to signal \" + signalName);\n          return;\n        }\n        object.__objectSignals__[signalIndex] = object.__objectSignals__[signalIndex] || [];\n        object.__objectSignals__[signalIndex].push(callback);\n        if (!isPropertyNotifySignal && signalName !== \"destroyed\") {\n          // only required for \"pure\" signals, handled separately for properties in propertyUpdate\n          // also note that we always get notified about the destroyed signal\n          webChannel.exec({\n            type: QWebChannelMessageTypes.connectToSignal,\n            object: object.__id__,\n            signal: signalIndex\n          });\n        }\n      },\n      disconnect: function (callback) {\n        if (typeof callback !== \"function\") {\n          console.error(\"Bad callback given to disconnect from signal \" + signalName);\n          return;\n        }\n        object.__objectSignals__[signalIndex] = object.__objectSignals__[signalIndex] || [];\n        var idx = object.__objectSignals__[signalIndex].indexOf(callback);\n        if (idx === -1) {\n          console.error(\"Cannot find connection of signal \" + signalName + \" to \" + callback.name);\n          return;\n        }\n        object.__objectSignals__[signalIndex].splice(idx, 1);\n        if (!isPropertyNotifySignal && object.__objectSignals__[signalIndex].length === 0) {\n          // only required for \"pure\" signals, handled separately for properties in propertyUpdate\n          webChannel.exec({\n            type: QWebChannelMessageTypes.disconnectFromSignal,\n            object: object.__id__,\n            signal: signalIndex\n          });\n        }\n      }\n    };\n  }\n\n  /**\r\n   * Invokes all callbacks for the given signalname. Also works for property notify callbacks.\r\n   */\n  function invokeSignalCallbacks(signalName, signalArgs) {\n    var connections = object.__objectSignals__[signalName];\n    if (connections) {\n      connections.forEach(function (callback) {\n        callback.apply(callback, signalArgs);\n      });\n    }\n  }\n  this.propertyUpdate = function (signals, propertyMap) {\n    // update property cache\n    for (var propertyIndex in propertyMap) {\n      var propertyValue = propertyMap[propertyIndex];\n      object.__propertyCache__[propertyIndex] = propertyValue;\n    }\n    for (var signalName in signals) {\n      // Invoke all callbacks, as signalEmitted() does not. This ensures the\n      // property cache is updated before the callbacks are invoked.\n      invokeSignalCallbacks(signalName, signals[signalName]);\n    }\n  };\n  this.signalEmitted = function (signalName, signalArgs) {\n    invokeSignalCallbacks(signalName, this.unwrapQObject(signalArgs));\n  };\n  function addMethod(methodData) {\n    var methodName = methodData[0];\n    var methodIdx = methodData[1];\n    object[methodName] = function () {\n      var args = [];\n      var callback;\n      for (var i = 0; i < arguments.length; ++i) {\n        var argument = arguments[i];\n        if (typeof argument === \"function\") callback = argument;else if (argument instanceof QObject && webChannel.objects[argument.__id__] !== undefined) args.push({\n          \"id\": argument.__id__\n        });else args.push(argument);\n      }\n      webChannel.exec({\n        \"type\": QWebChannelMessageTypes.invokeMethod,\n        \"object\": object.__id__,\n        \"method\": methodIdx,\n        \"args\": args\n      }, function (response) {\n        if (response !== undefined) {\n          var result = object.unwrapQObject(response);\n          if (callback) {\n            callback(result);\n          }\n        }\n      });\n    };\n  }\n  function bindGetterSetter(propertyInfo) {\n    var propertyIndex = propertyInfo[0];\n    var propertyName = propertyInfo[1];\n    var notifySignalData = propertyInfo[2];\n    // initialize property cache with current value\n    // NOTE: if this is an object, it is not directly unwrapped as it might\n    // reference other QObject that we do not know yet\n    object.__propertyCache__[propertyIndex] = propertyInfo[3];\n    if (notifySignalData) {\n      if (notifySignalData[0] === 1) {\n        // signal name is optimized away, reconstruct the actual name\n        notifySignalData[0] = propertyName + \"Changed\";\n      }\n      addSignal(notifySignalData, true);\n    }\n    Object.defineProperty(object, propertyName, {\n      configurable: true,\n      get: function () {\n        var propertyValue = object.__propertyCache__[propertyIndex];\n        if (propertyValue === undefined) {\n          // This shouldn't happen\n          console.warn(\"Undefined value in property cache for property \\\"\" + propertyName + \"\\\" in object \" + object.__id__);\n        }\n        return propertyValue;\n      },\n      set: function (value) {\n        if (value === undefined) {\n          console.warn(\"Property setter for \" + propertyName + \" called with undefined value!\");\n          return;\n        }\n        object.__propertyCache__[propertyIndex] = value;\n        var valueToSend = value;\n        if (valueToSend instanceof QObject && webChannel.objects[valueToSend.__id__] !== undefined) valueToSend = {\n          \"id\": valueToSend.__id__\n        };\n        webChannel.exec({\n          \"type\": QWebChannelMessageTypes.setProperty,\n          \"object\": object.__id__,\n          \"property\": propertyIndex,\n          \"value\": valueToSend\n        });\n      }\n    });\n  }\n\n  // ----------------------------------------------------------------------\n\n  data.methods.forEach(addMethod);\n  data.properties.forEach(bindGetterSetter);\n  data.signals.forEach(function (signal) {\n    addSignal(signal, false);\n  });\n  for (var name in data.enums) {\n    object[name] = data.enums[name];\n  }\n}\n\n//required for use with nodejs\nif (true) {\n  module.exports = {\n    QWebChannel: QWebChannel\n  };\n}\n\n//# sourceURL=webpack://xterm5-ishell/./src/common/js/qwebchannel.js?");

/***/ }),

/***/ "./src/main.js":
/*!*********************!*\
  !*** ./src/main.js ***!
  \*********************/
/***/ (function(__unused_webpack_module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var vue__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! vue */ \"./node_modules/vue/dist/vue.runtime.esm-bundler.js\");\n/* harmony import */ var _App_vue__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! ./App.vue */ \"./src/App.vue\");\n\n\n(0,vue__WEBPACK_IMPORTED_MODULE_0__.createApp)(_App_vue__WEBPACK_IMPORTED_MODULE_1__[\"default\"]).mount('#app');\n\n//# sourceURL=webpack://xterm5-ishell/./src/main.js?");

/***/ }),

/***/ "./node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use[1]!./node_modules/vue-loader/dist/stylePostLoader.js!./node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use[2]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css":
/*!****************************************************************************************************************************************************************************************************************************************************************************************************************!*\
  !*** ./node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use[1]!./node_modules/vue-loader/dist/stylePostLoader.js!./node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use[2]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css ***!
  \****************************************************************************************************************************************************************************************************************************************************************************************************************/
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _node_modules_css_loader_dist_runtime_noSourceMaps_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ../node_modules/css-loader/dist/runtime/noSourceMaps.js */ \"./node_modules/css-loader/dist/runtime/noSourceMaps.js\");\n/* harmony import */ var _node_modules_css_loader_dist_runtime_noSourceMaps_js__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(_node_modules_css_loader_dist_runtime_noSourceMaps_js__WEBPACK_IMPORTED_MODULE_0__);\n/* harmony import */ var _node_modules_css_loader_dist_runtime_api_js__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! ../node_modules/css-loader/dist/runtime/api.js */ \"./node_modules/css-loader/dist/runtime/api.js\");\n/* harmony import */ var _node_modules_css_loader_dist_runtime_api_js__WEBPACK_IMPORTED_MODULE_1___default = /*#__PURE__*/__webpack_require__.n(_node_modules_css_loader_dist_runtime_api_js__WEBPACK_IMPORTED_MODULE_1__);\n// Imports\n\n\nvar ___CSS_LOADER_EXPORT___ = _node_modules_css_loader_dist_runtime_api_js__WEBPACK_IMPORTED_MODULE_1___default()((_node_modules_css_loader_dist_runtime_noSourceMaps_js__WEBPACK_IMPORTED_MODULE_0___default()));\n// Module\n___CSS_LOADER_EXPORT___.push([module.id, \"\\nbody {\\n  margin: 0;\\n  padding: 0;\\n  background-color: #202935;\\n}\\n#terminal {\\n  margin: 0;\\n  padding: 0;\\n}\\n::-webkit-scrollbar {\\n  /*滚动条整体样式*/\\n  width: 6px;\\n  /*高宽分别对应横竖滚动条的尺寸*/\\n  height: 1px;\\n}\\n::-webkit-scrollbar-thumb {\\n  /*滚动条里面小方块*/\\n  border-radius: 10px;\\n  box-shadow: inset 0 0 5px rgba(97, 184, 179, 0.1);\\n  background: #202935;\\n}\\n::-webkit-scrollbar-track {\\n  /*滚动条里面轨道*/\\n  box-shadow: inset 0 0 5px rgba(87, 175, 187, 0.1);\\n  border-radius: 10px;\\n  background: #ededed;\\n}\\n\", \"\"]);\n// Exports\n/* harmony default export */ __webpack_exports__[\"default\"] = (___CSS_LOADER_EXPORT___);\n\n\n//# sourceURL=webpack://xterm5-ishell/./src/App.vue?./node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use%5B1%5D!./node_modules/vue-loader/dist/stylePostLoader.js!./node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use%5B2%5D!./node_modules/vue-loader/dist/index.js??ruleSet%5B0%5D.use%5B0%5D");

/***/ }),

/***/ "./src/App.vue":
/*!*********************!*\
  !*** ./src/App.vue ***!
  \*********************/
/***/ (function(__unused_webpack_module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _App_vue_vue_type_template_id_7ba5bd90__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ./App.vue?vue&type=template&id=7ba5bd90 */ \"./src/App.vue?vue&type=template&id=7ba5bd90\");\n/* harmony import */ var _App_vue_vue_type_script_lang_js__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! ./App.vue?vue&type=script&lang=js */ \"./src/App.vue?vue&type=script&lang=js\");\n/* harmony import */ var _App_vue_vue_type_style_index_0_id_7ba5bd90_lang_css__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! ./App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css */ \"./src/App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css\");\n/* harmony import */ var _node_modules_vue_loader_dist_exportHelper_js__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! ../node_modules/vue-loader/dist/exportHelper.js */ \"./node_modules/vue-loader/dist/exportHelper.js\");\n\n\n\n\n;\n\n\nconst __exports__ = /*#__PURE__*/(0,_node_modules_vue_loader_dist_exportHelper_js__WEBPACK_IMPORTED_MODULE_3__[\"default\"])(_App_vue_vue_type_script_lang_js__WEBPACK_IMPORTED_MODULE_1__[\"default\"], [['render',_App_vue_vue_type_template_id_7ba5bd90__WEBPACK_IMPORTED_MODULE_0__.render],['__file',\"src/App.vue\"]])\n/* hot reload */\nif (false) {}\n\n\n/* harmony default export */ __webpack_exports__[\"default\"] = (__exports__);\n\n//# sourceURL=webpack://xterm5-ishell/./src/App.vue?");

/***/ }),

/***/ "./src/App.vue?vue&type=script&lang=js":
/*!*********************************************!*\
  !*** ./src/App.vue?vue&type=script&lang=js ***!
  \*********************************************/
/***/ (function(__unused_webpack_module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony export */ __webpack_require__.d(__webpack_exports__, {\n/* harmony export */   \"default\": function() { return /* reexport safe */ _node_modules_babel_loader_lib_index_js_clonedRuleSet_40_use_0_node_modules_vue_loader_dist_index_js_ruleSet_0_use_0_App_vue_vue_type_script_lang_js__WEBPACK_IMPORTED_MODULE_0__[\"default\"]; }\n/* harmony export */ });\n/* harmony import */ var _node_modules_babel_loader_lib_index_js_clonedRuleSet_40_use_0_node_modules_vue_loader_dist_index_js_ruleSet_0_use_0_App_vue_vue_type_script_lang_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! -!../node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use[0]!../node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./App.vue?vue&type=script&lang=js */ \"./node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use[0]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=script&lang=js\");\n \n\n//# sourceURL=webpack://xterm5-ishell/./src/App.vue?");

/***/ }),

/***/ "./src/App.vue?vue&type=template&id=7ba5bd90":
/*!***************************************************!*\
  !*** ./src/App.vue?vue&type=template&id=7ba5bd90 ***!
  \***************************************************/
/***/ (function(__unused_webpack_module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony export */ __webpack_require__.d(__webpack_exports__, {\n/* harmony export */   render: function() { return /* reexport safe */ _node_modules_babel_loader_lib_index_js_clonedRuleSet_40_use_0_node_modules_vue_loader_dist_templateLoader_js_ruleSet_1_rules_3_node_modules_vue_loader_dist_index_js_ruleSet_0_use_0_App_vue_vue_type_template_id_7ba5bd90__WEBPACK_IMPORTED_MODULE_0__.render; }\n/* harmony export */ });\n/* harmony import */ var _node_modules_babel_loader_lib_index_js_clonedRuleSet_40_use_0_node_modules_vue_loader_dist_templateLoader_js_ruleSet_1_rules_3_node_modules_vue_loader_dist_index_js_ruleSet_0_use_0_App_vue_vue_type_template_id_7ba5bd90__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! -!../node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use[0]!../node_modules/vue-loader/dist/templateLoader.js??ruleSet[1].rules[3]!../node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./App.vue?vue&type=template&id=7ba5bd90 */ \"./node_modules/babel-loader/lib/index.js??clonedRuleSet-40.use[0]!./node_modules/vue-loader/dist/templateLoader.js??ruleSet[1].rules[3]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=template&id=7ba5bd90\");\n\n\n//# sourceURL=webpack://xterm5-ishell/./src/App.vue?");

/***/ }),

/***/ "./src/App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css":
/*!*****************************************************************!*\
  !*** ./src/App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css ***!
  \*****************************************************************/
/***/ (function(__unused_webpack_module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _node_modules_vue_style_loader_index_js_clonedRuleSet_12_use_0_node_modules_css_loader_dist_cjs_js_clonedRuleSet_12_use_1_node_modules_vue_loader_dist_stylePostLoader_js_node_modules_postcss_loader_dist_cjs_js_clonedRuleSet_12_use_2_node_modules_vue_loader_dist_index_js_ruleSet_0_use_0_App_vue_vue_type_style_index_0_id_7ba5bd90_lang_css__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! -!../node_modules/vue-style-loader/index.js??clonedRuleSet-12.use[0]!../node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use[1]!../node_modules/vue-loader/dist/stylePostLoader.js!../node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use[2]!../node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css */ \"./node_modules/vue-style-loader/index.js??clonedRuleSet-12.use[0]!./node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use[1]!./node_modules/vue-loader/dist/stylePostLoader.js!./node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use[2]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css\");\n/* harmony import */ var _node_modules_vue_style_loader_index_js_clonedRuleSet_12_use_0_node_modules_css_loader_dist_cjs_js_clonedRuleSet_12_use_1_node_modules_vue_loader_dist_stylePostLoader_js_node_modules_postcss_loader_dist_cjs_js_clonedRuleSet_12_use_2_node_modules_vue_loader_dist_index_js_ruleSet_0_use_0_App_vue_vue_type_style_index_0_id_7ba5bd90_lang_css__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(_node_modules_vue_style_loader_index_js_clonedRuleSet_12_use_0_node_modules_css_loader_dist_cjs_js_clonedRuleSet_12_use_1_node_modules_vue_loader_dist_stylePostLoader_js_node_modules_postcss_loader_dist_cjs_js_clonedRuleSet_12_use_2_node_modules_vue_loader_dist_index_js_ruleSet_0_use_0_App_vue_vue_type_style_index_0_id_7ba5bd90_lang_css__WEBPACK_IMPORTED_MODULE_0__);\n/* harmony reexport (unknown) */ var __WEBPACK_REEXPORT_OBJECT__ = {};\n/* harmony reexport (unknown) */ for(var __WEBPACK_IMPORT_KEY__ in _node_modules_vue_style_loader_index_js_clonedRuleSet_12_use_0_node_modules_css_loader_dist_cjs_js_clonedRuleSet_12_use_1_node_modules_vue_loader_dist_stylePostLoader_js_node_modules_postcss_loader_dist_cjs_js_clonedRuleSet_12_use_2_node_modules_vue_loader_dist_index_js_ruleSet_0_use_0_App_vue_vue_type_style_index_0_id_7ba5bd90_lang_css__WEBPACK_IMPORTED_MODULE_0__) if(__WEBPACK_IMPORT_KEY__ !== \"default\") __WEBPACK_REEXPORT_OBJECT__[__WEBPACK_IMPORT_KEY__] = function(key) { return _node_modules_vue_style_loader_index_js_clonedRuleSet_12_use_0_node_modules_css_loader_dist_cjs_js_clonedRuleSet_12_use_1_node_modules_vue_loader_dist_stylePostLoader_js_node_modules_postcss_loader_dist_cjs_js_clonedRuleSet_12_use_2_node_modules_vue_loader_dist_index_js_ruleSet_0_use_0_App_vue_vue_type_style_index_0_id_7ba5bd90_lang_css__WEBPACK_IMPORTED_MODULE_0__[key]; }.bind(0, __WEBPACK_IMPORT_KEY__)\n/* harmony reexport (unknown) */ __webpack_require__.d(__webpack_exports__, __WEBPACK_REEXPORT_OBJECT__);\n\n\n//# sourceURL=webpack://xterm5-ishell/./src/App.vue?");

/***/ }),

/***/ "./node_modules/vue-style-loader/index.js??clonedRuleSet-12.use[0]!./node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use[1]!./node_modules/vue-loader/dist/stylePostLoader.js!./node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use[2]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css":
/*!**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************!*\
  !*** ./node_modules/vue-style-loader/index.js??clonedRuleSet-12.use[0]!./node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use[1]!./node_modules/vue-loader/dist/stylePostLoader.js!./node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use[2]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css ***!
  \**********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
/***/ (function(module, __unused_webpack_exports, __webpack_require__) {

eval("// style-loader: Adds some css to the DOM by adding a <style> tag\n\n// load the styles\nvar content = __webpack_require__(/*! !!../node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use[1]!../node_modules/vue-loader/dist/stylePostLoader.js!../node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use[2]!../node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css */ \"./node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use[1]!./node_modules/vue-loader/dist/stylePostLoader.js!./node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use[2]!./node_modules/vue-loader/dist/index.js??ruleSet[0].use[0]!./src/App.vue?vue&type=style&index=0&id=7ba5bd90&lang=css\");\nif(content.__esModule) content = content.default;\nif(typeof content === 'string') content = [[module.id, content, '']];\nif(content.locals) module.exports = content.locals;\n// add the styles to the DOM\nvar add = (__webpack_require__(/*! !../node_modules/vue-style-loader/lib/addStylesClient.js */ \"./node_modules/vue-style-loader/lib/addStylesClient.js\")[\"default\"])\nvar update = add(\"7634ea42\", content, false, {\"sourceMap\":false,\"shadowMode\":false});\n// Hot Module Replacement\nif(false) {}\n\n//# sourceURL=webpack://xterm5-ishell/./src/App.vue?./node_modules/vue-style-loader/index.js??clonedRuleSet-12.use%5B0%5D!./node_modules/css-loader/dist/cjs.js??clonedRuleSet-12.use%5B1%5D!./node_modules/vue-loader/dist/stylePostLoader.js!./node_modules/postcss-loader/dist/cjs.js??clonedRuleSet-12.use%5B2%5D!./node_modules/vue-loader/dist/index.js??ruleSet%5B0%5D.use%5B0%5D");

/***/ })

/******/ 	});
/************************************************************************/
/******/ 	// The module cache
/******/ 	var __webpack_module_cache__ = {};
/******/ 	
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/ 		// Check if module is in cache
/******/ 		var cachedModule = __webpack_module_cache__[moduleId];
/******/ 		if (cachedModule !== undefined) {
/******/ 			return cachedModule.exports;
/******/ 		}
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = __webpack_module_cache__[moduleId] = {
/******/ 			id: moduleId,
/******/ 			// no module.loaded needed
/******/ 			exports: {}
/******/ 		};
/******/ 	
/******/ 		// Execute the module function
/******/ 		__webpack_modules__[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/ 	
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/ 	
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = __webpack_modules__;
/******/ 	
/************************************************************************/
/******/ 	/* webpack/runtime/chunk loaded */
/******/ 	!function() {
/******/ 		var deferred = [];
/******/ 		__webpack_require__.O = function(result, chunkIds, fn, priority) {
/******/ 			if(chunkIds) {
/******/ 				priority = priority || 0;
/******/ 				for(var i = deferred.length; i > 0 && deferred[i - 1][2] > priority; i--) deferred[i] = deferred[i - 1];
/******/ 				deferred[i] = [chunkIds, fn, priority];
/******/ 				return;
/******/ 			}
/******/ 			var notFulfilled = Infinity;
/******/ 			for (var i = 0; i < deferred.length; i++) {
/******/ 				var chunkIds = deferred[i][0];
/******/ 				var fn = deferred[i][1];
/******/ 				var priority = deferred[i][2];
/******/ 				var fulfilled = true;
/******/ 				for (var j = 0; j < chunkIds.length; j++) {
/******/ 					if ((priority & 1 === 0 || notFulfilled >= priority) && Object.keys(__webpack_require__.O).every(function(key) { return __webpack_require__.O[key](chunkIds[j]); })) {
/******/ 						chunkIds.splice(j--, 1);
/******/ 					} else {
/******/ 						fulfilled = false;
/******/ 						if(priority < notFulfilled) notFulfilled = priority;
/******/ 					}
/******/ 				}
/******/ 				if(fulfilled) {
/******/ 					deferred.splice(i--, 1)
/******/ 					var r = fn();
/******/ 					if (r !== undefined) result = r;
/******/ 				}
/******/ 			}
/******/ 			return result;
/******/ 		};
/******/ 	}();
/******/ 	
/******/ 	/* webpack/runtime/compat get default export */
/******/ 	!function() {
/******/ 		// getDefaultExport function for compatibility with non-harmony modules
/******/ 		__webpack_require__.n = function(module) {
/******/ 			var getter = module && module.__esModule ?
/******/ 				function() { return module['default']; } :
/******/ 				function() { return module; };
/******/ 			__webpack_require__.d(getter, { a: getter });
/******/ 			return getter;
/******/ 		};
/******/ 	}();
/******/ 	
/******/ 	/* webpack/runtime/define property getters */
/******/ 	!function() {
/******/ 		// define getter functions for harmony exports
/******/ 		__webpack_require__.d = function(exports, definition) {
/******/ 			for(var key in definition) {
/******/ 				if(__webpack_require__.o(definition, key) && !__webpack_require__.o(exports, key)) {
/******/ 					Object.defineProperty(exports, key, { enumerable: true, get: definition[key] });
/******/ 				}
/******/ 			}
/******/ 		};
/******/ 	}();
/******/ 	
/******/ 	/* webpack/runtime/global */
/******/ 	!function() {
/******/ 		__webpack_require__.g = (function() {
/******/ 			if (typeof globalThis === 'object') return globalThis;
/******/ 			try {
/******/ 				return this || new Function('return this')();
/******/ 			} catch (e) {
/******/ 				if (typeof window === 'object') return window;
/******/ 			}
/******/ 		})();
/******/ 	}();
/******/ 	
/******/ 	/* webpack/runtime/hasOwnProperty shorthand */
/******/ 	!function() {
/******/ 		__webpack_require__.o = function(obj, prop) { return Object.prototype.hasOwnProperty.call(obj, prop); }
/******/ 	}();
/******/ 	
/******/ 	/* webpack/runtime/make namespace object */
/******/ 	!function() {
/******/ 		// define __esModule on exports
/******/ 		__webpack_require__.r = function(exports) {
/******/ 			if(typeof Symbol !== 'undefined' && Symbol.toStringTag) {
/******/ 				Object.defineProperty(exports, Symbol.toStringTag, { value: 'Module' });
/******/ 			}
/******/ 			Object.defineProperty(exports, '__esModule', { value: true });
/******/ 		};
/******/ 	}();
/******/ 	
/******/ 	/* webpack/runtime/jsonp chunk loading */
/******/ 	!function() {
/******/ 		// no baseURI
/******/ 		
/******/ 		// object to store loaded and loading chunks
/******/ 		// undefined = chunk not loaded, null = chunk preloaded/prefetched
/******/ 		// [resolve, reject, Promise] = chunk loading, 0 = chunk loaded
/******/ 		var installedChunks = {
/******/ 			"app": 0
/******/ 		};
/******/ 		
/******/ 		// no chunk on demand loading
/******/ 		
/******/ 		// no prefetching
/******/ 		
/******/ 		// no preloaded
/******/ 		
/******/ 		// no HMR
/******/ 		
/******/ 		// no HMR manifest
/******/ 		
/******/ 		__webpack_require__.O.j = function(chunkId) { return installedChunks[chunkId] === 0; };
/******/ 		
/******/ 		// install a JSONP callback for chunk loading
/******/ 		var webpackJsonpCallback = function(parentChunkLoadingFunction, data) {
/******/ 			var chunkIds = data[0];
/******/ 			var moreModules = data[1];
/******/ 			var runtime = data[2];
/******/ 			// add "moreModules" to the modules object,
/******/ 			// then flag all "chunkIds" as loaded and fire callback
/******/ 			var moduleId, chunkId, i = 0;
/******/ 			if(chunkIds.some(function(id) { return installedChunks[id] !== 0; })) {
/******/ 				for(moduleId in moreModules) {
/******/ 					if(__webpack_require__.o(moreModules, moduleId)) {
/******/ 						__webpack_require__.m[moduleId] = moreModules[moduleId];
/******/ 					}
/******/ 				}
/******/ 				if(runtime) var result = runtime(__webpack_require__);
/******/ 			}
/******/ 			if(parentChunkLoadingFunction) parentChunkLoadingFunction(data);
/******/ 			for(;i < chunkIds.length; i++) {
/******/ 				chunkId = chunkIds[i];
/******/ 				if(__webpack_require__.o(installedChunks, chunkId) && installedChunks[chunkId]) {
/******/ 					installedChunks[chunkId][0]();
/******/ 				}
/******/ 				installedChunks[chunkId] = 0;
/******/ 			}
/******/ 			return __webpack_require__.O(result);
/******/ 		}
/******/ 		
/******/ 		var chunkLoadingGlobal = self["webpackChunkxterm5_ishell"] = self["webpackChunkxterm5_ishell"] || [];
/******/ 		chunkLoadingGlobal.forEach(webpackJsonpCallback.bind(null, 0));
/******/ 		chunkLoadingGlobal.push = webpackJsonpCallback.bind(null, chunkLoadingGlobal.push.bind(chunkLoadingGlobal));
/******/ 	}();
/******/ 	
/************************************************************************/
/******/ 	
/******/ 	// startup
/******/ 	// Load entry module and return exports
/******/ 	// This entry module depends on other loaded chunks and execution need to be delayed
/******/ 	var __webpack_exports__ = __webpack_require__.O(undefined, ["chunk-vendors"], function() { return __webpack_require__("./src/main.js"); })
/******/ 	__webpack_exports__ = __webpack_require__.O(__webpack_exports__);
/******/ 	
/******/ })()
;