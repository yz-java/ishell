/******/ (function(modules) { // webpackBootstrap
/******/ 	// install a JSONP callback for chunk loading
/******/ 	function webpackJsonpCallback(data) {
/******/ 		var chunkIds = data[0];
/******/ 		var moreModules = data[1];
/******/ 		var executeModules = data[2];
/******/
/******/ 		// add "moreModules" to the modules object,
/******/ 		// then flag all "chunkIds" as loaded and fire callback
/******/ 		var moduleId, chunkId, i = 0, resolves = [];
/******/ 		for(;i < chunkIds.length; i++) {
/******/ 			chunkId = chunkIds[i];
/******/ 			if(Object.prototype.hasOwnProperty.call(installedChunks, chunkId) && installedChunks[chunkId]) {
/******/ 				resolves.push(installedChunks[chunkId][0]);
/******/ 			}
/******/ 			installedChunks[chunkId] = 0;
/******/ 		}
/******/ 		for(moduleId in moreModules) {
/******/ 			if(Object.prototype.hasOwnProperty.call(moreModules, moduleId)) {
/******/ 				modules[moduleId] = moreModules[moduleId];
/******/ 			}
/******/ 		}
/******/ 		if(parentJsonpFunction) parentJsonpFunction(data);
/******/
/******/ 		while(resolves.length) {
/******/ 			resolves.shift()();
/******/ 		}
/******/
/******/ 		// add entry modules from loaded chunk to deferred list
/******/ 		deferredModules.push.apply(deferredModules, executeModules || []);
/******/
/******/ 		// run deferred modules when all chunks ready
/******/ 		return checkDeferredModules();
/******/ 	};
/******/ 	function checkDeferredModules() {
/******/ 		var result;
/******/ 		for(var i = 0; i < deferredModules.length; i++) {
/******/ 			var deferredModule = deferredModules[i];
/******/ 			var fulfilled = true;
/******/ 			for(var j = 1; j < deferredModule.length; j++) {
/******/ 				var depId = deferredModule[j];
/******/ 				if(installedChunks[depId] !== 0) fulfilled = false;
/******/ 			}
/******/ 			if(fulfilled) {
/******/ 				deferredModules.splice(i--, 1);
/******/ 				result = __webpack_require__(__webpack_require__.s = deferredModule[0]);
/******/ 			}
/******/ 		}
/******/
/******/ 		return result;
/******/ 	}
/******/
/******/ 	// The module cache
/******/ 	var installedModules = {};
/******/
/******/ 	// object to store loaded and loading chunks
/******/ 	// undefined = chunk not loaded, null = chunk preloaded/prefetched
/******/ 	// Promise = chunk loading, 0 = chunk loaded
/******/ 	var installedChunks = {
/******/ 		"app": 0
/******/ 	};
/******/
/******/ 	var deferredModules = [];
/******/
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/
/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId]) {
/******/ 			return installedModules[moduleId].exports;
/******/ 		}
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			i: moduleId,
/******/ 			l: false,
/******/ 			exports: {}
/******/ 		};
/******/
/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/
/******/ 		// Flag the module as loaded
/******/ 		module.l = true;
/******/
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/
/******/
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;
/******/
/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;
/******/
/******/ 	// define getter function for harmony exports
/******/ 	__webpack_require__.d = function(exports, name, getter) {
/******/ 		if(!__webpack_require__.o(exports, name)) {
/******/ 			Object.defineProperty(exports, name, { enumerable: true, get: getter });
/******/ 		}
/******/ 	};
/******/
/******/ 	// define __esModule on exports
/******/ 	__webpack_require__.r = function(exports) {
/******/ 		if(typeof Symbol !== 'undefined' && Symbol.toStringTag) {
/******/ 			Object.defineProperty(exports, Symbol.toStringTag, { value: 'Module' });
/******/ 		}
/******/ 		Object.defineProperty(exports, '__esModule', { value: true });
/******/ 	};
/******/
/******/ 	// create a fake namespace object
/******/ 	// mode & 1: value is a module id, require it
/******/ 	// mode & 2: merge all properties of value into the ns
/******/ 	// mode & 4: return value when already ns object
/******/ 	// mode & 8|1: behave like require
/******/ 	__webpack_require__.t = function(value, mode) {
/******/ 		if(mode & 1) value = __webpack_require__(value);
/******/ 		if(mode & 8) return value;
/******/ 		if((mode & 4) && typeof value === 'object' && value && value.__esModule) return value;
/******/ 		var ns = Object.create(null);
/******/ 		__webpack_require__.r(ns);
/******/ 		Object.defineProperty(ns, 'default', { enumerable: true, value: value });
/******/ 		if(mode & 2 && typeof value != 'string') for(var key in value) __webpack_require__.d(ns, key, function(key) { return value[key]; }.bind(null, key));
/******/ 		return ns;
/******/ 	};
/******/
/******/ 	// getDefaultExport function for compatibility with non-harmony modules
/******/ 	__webpack_require__.n = function(module) {
/******/ 		var getter = module && module.__esModule ?
/******/ 			function getDefault() { return module['default']; } :
/******/ 			function getModuleExports() { return module; };
/******/ 		__webpack_require__.d(getter, 'a', getter);
/******/ 		return getter;
/******/ 	};
/******/
/******/ 	// Object.prototype.hasOwnProperty.call
/******/ 	__webpack_require__.o = function(object, property) { return Object.prototype.hasOwnProperty.call(object, property); };
/******/
/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "qrc:/html/xterm4/";
/******/
/******/ 	var jsonpArray = window["webpackJsonp"] = window["webpackJsonp"] || [];
/******/ 	var oldJsonpFunction = jsonpArray.push.bind(jsonpArray);
/******/ 	jsonpArray.push = webpackJsonpCallback;
/******/ 	jsonpArray = jsonpArray.slice();
/******/ 	for(var i = 0; i < jsonpArray.length; i++) webpackJsonpCallback(jsonpArray[i]);
/******/ 	var parentJsonpFunction = oldJsonpFunction;
/******/
/******/
/******/ 	// add entry module to deferred list
/******/ 	deferredModules.push([0,"chunk-vendors"]);
/******/ 	// run deferred modules when ready
/******/ 	return checkDeferredModules();
/******/ })
/************************************************************************/
/******/ ({

/***/ "./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_babel-loader@8.2.2@babel-loader/lib/index.js!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/index.js?!./src/App.vue?vue&type=script&lang=js&":
/*!********************************************************************************************************************************************************************************************************************************************************************************************************************!*\
  !*** ./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--12-0!./node_modules/_babel-loader@8.2.2@babel-loader/lib!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!./node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options!./src/App.vue?vue&type=script&lang=js& ***!
  \********************************************************************************************************************************************************************************************************************************************************************************************************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* WEBPACK VAR INJECTION */(function($) {/* harmony import */ var core_js_modules_es_regexp_exec_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! core-js/modules/es.regexp.exec.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.regexp.exec.js\");\n/* harmony import */ var core_js_modules_es_regexp_exec_js__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(core_js_modules_es_regexp_exec_js__WEBPACK_IMPORTED_MODULE_0__);\n/* harmony import */ var core_js_modules_es_string_search_js__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! core-js/modules/es.string.search.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.string.search.js\");\n/* harmony import */ var core_js_modules_es_string_search_js__WEBPACK_IMPORTED_MODULE_1___default = /*#__PURE__*/__webpack_require__.n(core_js_modules_es_string_search_js__WEBPACK_IMPORTED_MODULE_1__);\n/* harmony import */ var xterm_css_xterm_css__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! xterm/css/xterm.css */ \"./node_modules/_xterm@4.14.1@xterm/css/xterm.css\");\n/* harmony import */ var xterm_css_xterm_css__WEBPACK_IMPORTED_MODULE_2___default = /*#__PURE__*/__webpack_require__.n(xterm_css_xterm_css__WEBPACK_IMPORTED_MODULE_2__);\n/* harmony import */ var _common_js_qwebchannel_js__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! ./common/js/qwebchannel.js */ \"./src/common/js/qwebchannel.js\");\n/* harmony import */ var _common_js_qwebchannel_js__WEBPACK_IMPORTED_MODULE_3___default = /*#__PURE__*/__webpack_require__.n(_common_js_qwebchannel_js__WEBPACK_IMPORTED_MODULE_3__);\n/* harmony import */ var xterm__WEBPACK_IMPORTED_MODULE_4__ = __webpack_require__(/*! xterm */ \"./node_modules/_xterm@4.14.1@xterm/lib/xterm.js\");\n/* harmony import */ var xterm__WEBPACK_IMPORTED_MODULE_4___default = /*#__PURE__*/__webpack_require__.n(xterm__WEBPACK_IMPORTED_MODULE_4__);\n/* harmony import */ var xterm_addon_fit__WEBPACK_IMPORTED_MODULE_5__ = __webpack_require__(/*! xterm-addon-fit */ \"./node_modules/_xterm-addon-fit@0.5.0@xterm-addon-fit/lib/xterm-addon-fit.js\");\n/* harmony import */ var xterm_addon_fit__WEBPACK_IMPORTED_MODULE_5___default = /*#__PURE__*/__webpack_require__.n(xterm_addon_fit__WEBPACK_IMPORTED_MODULE_5__);\n\n\n//\n//\n//\n//\n//\n//\n//\n//\n// import HelloWorld from './components/HelloWorld.vue'\n\n\n\n\n/* harmony default export */ __webpack_exports__[\"default\"] = ({\n  name: \"App\",\n  components: {// HelloWorld\n  },\n  data: function data() {\n    return {\n      xterm: null,\n      fitAddon: null,\n      rows: 40,\n      cols: 100,\n      clientId: null\n    };\n  },\n  created: function created() {\n    window.setClientId = this.setClientId;\n  },\n  mounted: function mounted() {\n    var _this = this;\n\n    window.setClientId = this.setClientId;\n    new _common_js_qwebchannel_js__WEBPACK_IMPORTED_MODULE_3___default.a.QWebChannel(qt.webChannelTransport, function (channel) {\n      window.core = channel.objects.core;\n    });\n    var terminal = new xterm__WEBPACK_IMPORTED_MODULE_4__[\"Terminal\"]({\n      termName: \"xterm\",\n      useStyle: true,\n      convertEol: true,\n      screenKeys: true,\n      cursorBlink: true,\n      visualBell: true,\n      colors: xterm__WEBPACK_IMPORTED_MODULE_4__[\"Terminal\"].xtermColors,\n      rendererType: \"dom\"\n    });\n    this.xterm = terminal;\n    this.fitAddon = new xterm_addon_fit__WEBPACK_IMPORTED_MODULE_5__[\"FitAddon\"]();\n    terminal.loadAddon(this.fitAddon);\n    terminal.open(this.$refs[\"terminal\"]);\n    this.fitAddon.fit(); // terminal.writeln(\"hello world\")\n\n    terminal.focus();\n\n    terminal.prompt = function () {\n      terminal.write(\"\\r\\n$ \");\n    };\n\n    var rc = this.getRowsAndCols();\n    terminal.resize(rc.cols, rc.rows);\n    window.addEventListener(\"resize\", function () {\n      _this.fitAddon.fit();\n\n      var rc = _this.getRowsAndCols();\n\n      terminal.resize(rc.cols, rc.rows);\n\n      _this.requestPtySize();\n    });\n    if (terminal._initialized) return; // 初始化\n\n    terminal._initialized = true; // terminal.prompt();\n    // 添加事件监听器，支持输入方法\n    // terminal.onKey((e) => {\n    //   const printable =\n    //     !e.domEvent.altKey &&\n    //     !e.domEvent.altGraphKey &&\n    //     !e.domEvent.ctrlKey &&\n    //     !e.domEvent.metaKey;\n    //   if (e.domEvent.keyCode === 13) {\n    //     terminal.prompt();\n    //   } else if (e.domEvent.keyCode === 8) {\n    //     // back 删除的情况\n    //     if (terminal._core.buffer.x > 2) {\n    //       terminal.write(\"\\b \\b\");\n    //     }\n    //   } else if (printable) {\n    //     // terminal.write(e.key);\n    //   }\n    //   console.log(1, \"print\", e.key);\n    // });\n\n    terminal.onData(function (key) {\n      // terminal.write(key);\n      window.core.recieveJsMessage(key);\n    });\n  },\n  methods: {\n    getRowsAndCols: function getRowsAndCols() {\n      this.xterm.scrollToBottom();\n      var height = document.documentElement.clientHeight;\n      var rows = parseInt(height / 17) - 1;\n      var span = $(\"<span>\", {\n        text: \"qwertyuiopasdfghjklzxcvbnm\"\n      });\n      $(\"#terminal\").append(span);\n      var span_width = span.width(); // var span_height = span.height();\n\n      span.remove();\n      var width = document.body.clientWidth;\n      var cols = parseInt(width / (span_width / 26));\n      return {\n        rows: rows,\n        cols: cols\n      };\n    },\n    requestPtySize: function requestPtySize() {\n      var rc = this.getRowsAndCols();\n      var ss = rc.rows + \",\" + rc.cols;\n\n      if (window.core != undefined) {\n        window.core.setChannelRequestPtySize(ss);\n      }\n    },\n    setClientId: function setClientId(cId) {\n      this.clientId = cId;\n      this.open_websocket();\n      setTimeout(function () {\n        window.core.ssh2connect(\"\");\n      }, 2000);\n    },\n    open_websocket: function open_websocket() {\n      var _this2 = this;\n\n      if (location.search != \"\") var baseUrl = /[?&]webChannelBaseUrl=([A-Za-z0-9\\-:/\\.]+)/.exec(location.search)[1];else var baseUrl = \"ws://localhost:12345\";\n      var socket = new WebSocket(baseUrl);\n\n      socket.onclose = function () {\n        console.error(\"web channel closed\");\n      };\n\n      socket.onerror = function (error) {\n        console.error(\"web channel error: \" + error);\n      };\n\n      socket.onopen = function () {\n        socket.send(_this2.clientId);\n      };\n\n      socket.onmessage = function (evt) {\n        var received_msg = evt.data;\n\n        _this2.xterm.write(received_msg);\n      };\n    }\n  }\n});\n/* WEBPACK VAR INJECTION */}.call(this, __webpack_require__(/*! jquery */ \"./node_modules/_jquery@3.6.0@jquery/dist/jquery.js\")))\n\n//# sourceURL=webpack:///./src/App.vue?./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--12-0!./node_modules/_babel-loader@8.2.2@babel-loader/lib!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!./node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options");

/***/ }),

/***/ "./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?{\"cacheDirectory\":\"node_modules/.cache/vue-loader\",\"cacheIdentifier\":\"0a689e2a-vue-loader-template\"}!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/templateLoader.js?!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/index.js?!./src/App.vue?vue&type=template&id=7ba5bd90&":
/*!***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************!*\
  !*** ./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?{"cacheDirectory":"node_modules/.cache/vue-loader","cacheIdentifier":"0a689e2a-vue-loader-template"}!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/templateLoader.js??vue-loader-options!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!./node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options!./src/App.vue?vue&type=template&id=7ba5bd90& ***!
  \***************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
/*! exports provided: render, staticRenderFns */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, \"render\", function() { return render; });\n/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, \"staticRenderFns\", function() { return staticRenderFns; });\nvar render = function() {\n  var _vm = this\n  var _h = _vm.$createElement\n  var _c = _vm._self._c || _h\n  return _c(\"div\", { attrs: { id: \"app\" } }, [\n    _c(\"div\", { ref: \"terminal\", attrs: { id: \"terminal\" } })\n  ])\n}\nvar staticRenderFns = []\nrender._withStripped = true\n\n\n\n//# sourceURL=webpack:///./src/App.vue?./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?%7B%22cacheDirectory%22:%22node_modules/.cache/vue-loader%22,%22cacheIdentifier%22:%220a689e2a-vue-loader-template%22%7D!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/templateLoader.js??vue-loader-options!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!./node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options");

/***/ }),

/***/ "./node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!./node_modules/_postcss-loader@3.0.0@postcss-loader/src/index.js?!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/index.js?!./src/App.vue?vue&type=style&index=0&lang=css&":
/*!*********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************!*\
  !*** ./node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js??ref--6-oneOf-1-1!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!./node_modules/_postcss-loader@3.0.0@postcss-loader/src??ref--6-oneOf-1-2!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!./node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options!./src/App.vue?vue&type=style&index=0&lang=css& ***!
  \*********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

eval("// Imports\nvar ___CSS_LOADER_API_IMPORT___ = __webpack_require__(/*! ../node_modules/_css-loader@3.6.0@css-loader/dist/runtime/api.js */ \"./node_modules/_css-loader@3.6.0@css-loader/dist/runtime/api.js\");\nexports = ___CSS_LOADER_API_IMPORT___(false);\n// Module\nexports.push([module.i, \"\\nbody{\\n  margin: 0;\\n  padding: 0;\\n  overflow: hidden;\\n}\\n/* #app {\\n  font-family: Avenir, Helvetica, Arial, sans-serif;\\n  -webkit-font-smoothing: antialiased;\\n  -moz-osx-font-smoothing: grayscale;\\n  text-align: center;\\n  color: #2c3e50;\\n  margin-top: 60px;\\n  margin: 0;\\n  padding: 0;\\n} */\\n\", \"\"]);\n// Exports\nmodule.exports = exports;\n\n\n//# sourceURL=webpack:///./src/App.vue?./node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js??ref--6-oneOf-1-1!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!./node_modules/_postcss-loader@3.0.0@postcss-loader/src??ref--6-oneOf-1-2!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!./node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options");

/***/ }),

/***/ "./node_modules/_vue-style-loader@4.1.3@vue-style-loader/index.js?!./node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!./node_modules/_postcss-loader@3.0.0@postcss-loader/src/index.js?!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/index.js?!./src/App.vue?vue&type=style&index=0&lang=css&":
/*!***********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************!*\
  !*** ./node_modules/_vue-style-loader@4.1.3@vue-style-loader??ref--6-oneOf-1-0!./node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js??ref--6-oneOf-1-1!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!./node_modules/_postcss-loader@3.0.0@postcss-loader/src??ref--6-oneOf-1-2!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!./node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options!./src/App.vue?vue&type=style&index=0&lang=css& ***!
  \***********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

eval("// style-loader: Adds some css to the DOM by adding a <style> tag\n\n// load the styles\nvar content = __webpack_require__(/*! !../node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js??ref--6-oneOf-1-1!../node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!../node_modules/_postcss-loader@3.0.0@postcss-loader/src??ref--6-oneOf-1-2!../node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!../node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options!./App.vue?vue&type=style&index=0&lang=css& */ \"./node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!./node_modules/_postcss-loader@3.0.0@postcss-loader/src/index.js?!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/index.js?!./src/App.vue?vue&type=style&index=0&lang=css&\");\nif(content.__esModule) content = content.default;\nif(typeof content === 'string') content = [[module.i, content, '']];\nif(content.locals) module.exports = content.locals;\n// add the styles to the DOM\nvar add = __webpack_require__(/*! ../node_modules/_vue-style-loader@4.1.3@vue-style-loader/lib/addStylesClient.js */ \"./node_modules/_vue-style-loader@4.1.3@vue-style-loader/lib/addStylesClient.js\").default\nvar update = add(\"6a9ccb42\", content, false, {\"sourceMap\":false,\"shadowMode\":false});\n// Hot Module Replacement\nif(false) {}\n\n//# sourceURL=webpack:///./src/App.vue?./node_modules/_vue-style-loader@4.1.3@vue-style-loader??ref--6-oneOf-1-0!./node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js??ref--6-oneOf-1-1!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!./node_modules/_postcss-loader@3.0.0@postcss-loader/src??ref--6-oneOf-1-2!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!./node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options");

/***/ }),

/***/ "./src/App.vue":
/*!*********************!*\
  !*** ./src/App.vue ***!
  \*********************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _App_vue_vue_type_template_id_7ba5bd90___WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ./App.vue?vue&type=template&id=7ba5bd90& */ \"./src/App.vue?vue&type=template&id=7ba5bd90&\");\n/* harmony import */ var _App_vue_vue_type_script_lang_js___WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! ./App.vue?vue&type=script&lang=js& */ \"./src/App.vue?vue&type=script&lang=js&\");\n/* empty/unused harmony star reexport *//* harmony import */ var _App_vue_vue_type_style_index_0_lang_css___WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! ./App.vue?vue&type=style&index=0&lang=css& */ \"./src/App.vue?vue&type=style&index=0&lang=css&\");\n/* harmony import */ var _node_modules_vue_loader_15_9_8_vue_loader_lib_runtime_componentNormalizer_js__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! ../node_modules/_vue-loader@15.9.8@vue-loader/lib/runtime/componentNormalizer.js */ \"./node_modules/_vue-loader@15.9.8@vue-loader/lib/runtime/componentNormalizer.js\");\n\n\n\n\n\n\n/* normalize component */\n\nvar component = Object(_node_modules_vue_loader_15_9_8_vue_loader_lib_runtime_componentNormalizer_js__WEBPACK_IMPORTED_MODULE_3__[\"default\"])(\n  _App_vue_vue_type_script_lang_js___WEBPACK_IMPORTED_MODULE_1__[\"default\"],\n  _App_vue_vue_type_template_id_7ba5bd90___WEBPACK_IMPORTED_MODULE_0__[\"render\"],\n  _App_vue_vue_type_template_id_7ba5bd90___WEBPACK_IMPORTED_MODULE_0__[\"staticRenderFns\"],\n  false,\n  null,\n  null,\n  null\n  \n)\n\n/* hot reload */\nif (false) { var api; }\ncomponent.options.__file = \"src/App.vue\"\n/* harmony default export */ __webpack_exports__[\"default\"] = (component.exports);\n\n//# sourceURL=webpack:///./src/App.vue?");

/***/ }),

/***/ "./src/App.vue?vue&type=script&lang=js&":
/*!**********************************************!*\
  !*** ./src/App.vue?vue&type=script&lang=js& ***!
  \**********************************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_12_0_node_modules_babel_loader_8_2_2_babel_loader_lib_index_js_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_script_lang_js___WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! -!../node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--12-0!../node_modules/_babel-loader@8.2.2@babel-loader/lib!../node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!../node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options!./App.vue?vue&type=script&lang=js& */ \"./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_babel-loader@8.2.2@babel-loader/lib/index.js!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/index.js?!./src/App.vue?vue&type=script&lang=js&\");\n/* empty/unused harmony star reexport */ /* harmony default export */ __webpack_exports__[\"default\"] = (_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_12_0_node_modules_babel_loader_8_2_2_babel_loader_lib_index_js_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_script_lang_js___WEBPACK_IMPORTED_MODULE_0__[\"default\"]); \n\n//# sourceURL=webpack:///./src/App.vue?");

/***/ }),

/***/ "./src/App.vue?vue&type=style&index=0&lang=css&":
/*!******************************************************!*\
  !*** ./src/App.vue?vue&type=style&index=0&lang=css& ***!
  \******************************************************/
/*! no static exports found */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _node_modules_vue_style_loader_4_1_3_vue_style_loader_index_js_ref_6_oneOf_1_0_node_modules_css_loader_3_6_0_css_loader_dist_cjs_js_ref_6_oneOf_1_1_node_modules_vue_loader_15_9_8_vue_loader_lib_loaders_stylePostLoader_js_node_modules_postcss_loader_3_0_0_postcss_loader_src_index_js_ref_6_oneOf_1_2_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_style_index_0_lang_css___WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! -!../node_modules/_vue-style-loader@4.1.3@vue-style-loader??ref--6-oneOf-1-0!../node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js??ref--6-oneOf-1-1!../node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!../node_modules/_postcss-loader@3.0.0@postcss-loader/src??ref--6-oneOf-1-2!../node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!../node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options!./App.vue?vue&type=style&index=0&lang=css& */ \"./node_modules/_vue-style-loader@4.1.3@vue-style-loader/index.js?!./node_modules/_css-loader@3.6.0@css-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/stylePostLoader.js!./node_modules/_postcss-loader@3.0.0@postcss-loader/src/index.js?!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/index.js?!./src/App.vue?vue&type=style&index=0&lang=css&\");\n/* harmony import */ var _node_modules_vue_style_loader_4_1_3_vue_style_loader_index_js_ref_6_oneOf_1_0_node_modules_css_loader_3_6_0_css_loader_dist_cjs_js_ref_6_oneOf_1_1_node_modules_vue_loader_15_9_8_vue_loader_lib_loaders_stylePostLoader_js_node_modules_postcss_loader_3_0_0_postcss_loader_src_index_js_ref_6_oneOf_1_2_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_style_index_0_lang_css___WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(_node_modules_vue_style_loader_4_1_3_vue_style_loader_index_js_ref_6_oneOf_1_0_node_modules_css_loader_3_6_0_css_loader_dist_cjs_js_ref_6_oneOf_1_1_node_modules_vue_loader_15_9_8_vue_loader_lib_loaders_stylePostLoader_js_node_modules_postcss_loader_3_0_0_postcss_loader_src_index_js_ref_6_oneOf_1_2_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_style_index_0_lang_css___WEBPACK_IMPORTED_MODULE_0__);\n/* harmony reexport (unknown) */ for(var __WEBPACK_IMPORT_KEY__ in _node_modules_vue_style_loader_4_1_3_vue_style_loader_index_js_ref_6_oneOf_1_0_node_modules_css_loader_3_6_0_css_loader_dist_cjs_js_ref_6_oneOf_1_1_node_modules_vue_loader_15_9_8_vue_loader_lib_loaders_stylePostLoader_js_node_modules_postcss_loader_3_0_0_postcss_loader_src_index_js_ref_6_oneOf_1_2_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_style_index_0_lang_css___WEBPACK_IMPORTED_MODULE_0__) if([\"default\"].indexOf(__WEBPACK_IMPORT_KEY__) < 0) (function(key) { __webpack_require__.d(__webpack_exports__, key, function() { return _node_modules_vue_style_loader_4_1_3_vue_style_loader_index_js_ref_6_oneOf_1_0_node_modules_css_loader_3_6_0_css_loader_dist_cjs_js_ref_6_oneOf_1_1_node_modules_vue_loader_15_9_8_vue_loader_lib_loaders_stylePostLoader_js_node_modules_postcss_loader_3_0_0_postcss_loader_src_index_js_ref_6_oneOf_1_2_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_style_index_0_lang_css___WEBPACK_IMPORTED_MODULE_0__[key]; }) }(__WEBPACK_IMPORT_KEY__));\n\n\n//# sourceURL=webpack:///./src/App.vue?");

/***/ }),

/***/ "./src/App.vue?vue&type=template&id=7ba5bd90&":
/*!****************************************************!*\
  !*** ./src/App.vue?vue&type=template&id=7ba5bd90& ***!
  \****************************************************/
/*! exports provided: render, staticRenderFns */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_cacheDirectory_node_modules_cache_vue_loader_cacheIdentifier_0a689e2a_vue_loader_template_node_modules_vue_loader_15_9_8_vue_loader_lib_loaders_templateLoader_js_vue_loader_options_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_template_id_7ba5bd90___WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! -!../node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?{\"cacheDirectory\":\"node_modules/.cache/vue-loader\",\"cacheIdentifier\":\"0a689e2a-vue-loader-template\"}!../node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/templateLoader.js??vue-loader-options!../node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js??ref--0-0!../node_modules/_vue-loader@15.9.8@vue-loader/lib??vue-loader-options!./App.vue?vue&type=template&id=7ba5bd90& */ \"./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?{\\\"cacheDirectory\\\":\\\"node_modules/.cache/vue-loader\\\",\\\"cacheIdentifier\\\":\\\"0a689e2a-vue-loader-template\\\"}!./node_modules/_vue-loader@15.9.8@vue-loader/lib/loaders/templateLoader.js?!./node_modules/_cache-loader@4.1.0@cache-loader/dist/cjs.js?!./node_modules/_vue-loader@15.9.8@vue-loader/lib/index.js?!./src/App.vue?vue&type=template&id=7ba5bd90&\");\n/* harmony reexport (safe) */ __webpack_require__.d(__webpack_exports__, \"render\", function() { return _node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_cacheDirectory_node_modules_cache_vue_loader_cacheIdentifier_0a689e2a_vue_loader_template_node_modules_vue_loader_15_9_8_vue_loader_lib_loaders_templateLoader_js_vue_loader_options_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_template_id_7ba5bd90___WEBPACK_IMPORTED_MODULE_0__[\"render\"]; });\n\n/* harmony reexport (safe) */ __webpack_require__.d(__webpack_exports__, \"staticRenderFns\", function() { return _node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_cacheDirectory_node_modules_cache_vue_loader_cacheIdentifier_0a689e2a_vue_loader_template_node_modules_vue_loader_15_9_8_vue_loader_lib_loaders_templateLoader_js_vue_loader_options_node_modules_cache_loader_4_1_0_cache_loader_dist_cjs_js_ref_0_0_node_modules_vue_loader_15_9_8_vue_loader_lib_index_js_vue_loader_options_App_vue_vue_type_template_id_7ba5bd90___WEBPACK_IMPORTED_MODULE_0__[\"staticRenderFns\"]; });\n\n\n\n//# sourceURL=webpack:///./src/App.vue?");

/***/ }),

/***/ "./src/common/js/qwebchannel.js":
/*!**************************************!*\
  !*** ./src/common/js/qwebchannel.js ***!
  \**************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";
eval("/* WEBPACK VAR INJECTION */(function(module) {/****************************************************************************\r\n**\r\n** Copyright (C) 2016 The Qt Company Ltd.\r\n** Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Milian Wolff <milian.wolff@kdab.com>\r\n** Contact: https://www.qt.io/licensing/\r\n**\r\n** This file is part of the QtWebChannel module of the Qt Toolkit.\r\n**\r\n** $QT_BEGIN_LICENSE:LGPL$\r\n** Commercial License Usage\r\n** Licensees holding valid commercial Qt licenses may use this file in\r\n** accordance with the commercial license agreement provided with the\r\n** Software or, alternatively, in accordance with the terms contained in\r\n** a written agreement between you and The Qt Company. For licensing terms\r\n** and conditions see https://www.qt.io/terms-conditions. For further\r\n** information use the contact form at https://www.qt.io/contact-us.\r\n**\r\n** GNU Lesser General Public License Usage\r\n** Alternatively, this file may be used under the terms of the GNU Lesser\r\n** General Public License version 3 as published by the Free Software\r\n** Foundation and appearing in the file LICENSE.LGPL3 included in the\r\n** packaging of this file. Please review the following information to\r\n** ensure the GNU Lesser General Public License version 3 requirements\r\n** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.\r\n**\r\n** GNU General Public License Usage\r\n** Alternatively, this file may be used under the terms of the GNU\r\n** General Public License version 2.0 or (at your option) the GNU General\r\n** Public license version 3 or any later version approved by the KDE Free\r\n** Qt Foundation. The licenses are as published by the Free Software\r\n** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3\r\n** included in the packaging of this file. Please review the following\r\n** information to ensure the GNU General Public License requirements will\r\n** be met: https://www.gnu.org/licenses/gpl-2.0.html and\r\n** https://www.gnu.org/licenses/gpl-3.0.html.\r\n**\r\n** $QT_END_LICENSE$\r\n**\r\n****************************************************************************/\n\n\nvar _typeof = __webpack_require__(/*! ./node_modules/_@babel_runtime@7.15.4@@babel/runtime/helpers/typeof */ \"./node_modules/_@babel_runtime@7.15.4@@babel/runtime/helpers/typeof.js\").default;\n\n__webpack_require__(/*! core-js/modules/es.regexp.exec.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.regexp.exec.js\");\n\n__webpack_require__(/*! core-js/modules/es.number.constructor.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.number.constructor.js\");\n\n__webpack_require__(/*! core-js/modules/es.function.name.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.function.name.js\");\n\n__webpack_require__(/*! core-js/modules/es.array.splice.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.array.splice.js\");\n\n__webpack_require__(/*! core-js/modules/web.dom-collections.for-each.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/web.dom-collections.for-each.js\");\n\nvar QWebChannelMessageTypes = {\n  signal: 1,\n  propertyUpdate: 2,\n  init: 3,\n  idle: 4,\n  debug: 5,\n  invokeMethod: 6,\n  connectToSignal: 7,\n  disconnectFromSignal: 8,\n  setProperty: 9,\n  response: 10\n};\n\nvar QWebChannel = function QWebChannel(transport, initCallback) {\n  if (_typeof(transport) !== \"object\" || typeof transport.send !== \"function\") {\n    console.error(\"The QWebChannel expects a transport object with a send function and onmessage callback property.\" + \" Given is: transport: \" + _typeof(transport) + \", transport.send: \" + _typeof(transport.send));\n    return;\n  }\n\n  var channel = this;\n  this.transport = transport;\n\n  this.send = function (data) {\n    if (typeof data !== \"string\") {\n      data = JSON.stringify(data);\n    }\n\n    channel.transport.send(data);\n  };\n\n  this.transport.onmessage = function (message) {\n    var data = message.data;\n\n    if (typeof data === \"string\") {\n      data = JSON.parse(data);\n    }\n\n    switch (data.type) {\n      case QWebChannelMessageTypes.signal:\n        channel.handleSignal(data);\n        break;\n\n      case QWebChannelMessageTypes.response:\n        channel.handleResponse(data);\n        break;\n\n      case QWebChannelMessageTypes.propertyUpdate:\n        channel.handlePropertyUpdate(data);\n        break;\n\n      default:\n        console.error(\"invalid message received:\", message.data);\n        break;\n    }\n  };\n\n  this.execCallbacks = {};\n  this.execId = 0;\n\n  this.exec = function (data, callback) {\n    if (!callback) {\n      // if no callback is given, send directly\n      channel.send(data);\n      return;\n    }\n\n    if (channel.execId === Number.MAX_VALUE) {\n      // wrap\n      channel.execId = Number.MIN_VALUE;\n    }\n\n    if (data.hasOwnProperty(\"id\")) {\n      console.error(\"Cannot exec message with property id: \" + JSON.stringify(data));\n      return;\n    }\n\n    data.id = channel.execId++;\n    channel.execCallbacks[data.id] = callback;\n    channel.send(data);\n  };\n\n  this.objects = {};\n\n  this.handleSignal = function (message) {\n    var object = channel.objects[message.object];\n\n    if (object) {\n      object.signalEmitted(message.signal, message.args);\n    } else {\n      console.warn(\"Unhandled signal: \" + message.object + \"::\" + message.signal);\n    }\n  };\n\n  this.handleResponse = function (message) {\n    if (!message.hasOwnProperty(\"id\")) {\n      console.error(\"Invalid response message received: \", JSON.stringify(message));\n      return;\n    }\n\n    channel.execCallbacks[message.id](message.data);\n    delete channel.execCallbacks[message.id];\n  };\n\n  this.handlePropertyUpdate = function (message) {\n    for (var i in message.data) {\n      var data = message.data[i];\n      var object = channel.objects[data.object];\n\n      if (object) {\n        object.propertyUpdate(data.signals, data.properties);\n      } else {\n        console.warn(\"Unhandled property update: \" + data.object + \"::\" + data.signal);\n      }\n    }\n\n    channel.exec({\n      type: QWebChannelMessageTypes.idle\n    });\n  };\n\n  this.debug = function (message) {\n    channel.send({\n      type: QWebChannelMessageTypes.debug,\n      data: message\n    });\n  };\n\n  channel.exec({\n    type: QWebChannelMessageTypes.init\n  }, function (data) {\n    for (var objectName in data) {\n      var object = new QObject(objectName, data[objectName], channel);\n    } // now unwrap properties, which might reference other registered objects\n\n\n    for (var objectName in channel.objects) {\n      channel.objects[objectName].unwrapProperties();\n    }\n\n    if (initCallback) {\n      initCallback(channel);\n    }\n\n    channel.exec({\n      type: QWebChannelMessageTypes.idle\n    });\n  });\n};\n\nfunction QObject(name, data, webChannel) {\n  this.__id__ = name;\n  webChannel.objects[name] = this; // List of callbacks that get invoked upon signal emission\n\n  this.__objectSignals__ = {}; // Cache of all properties, updated when a notify signal is emitted\n\n  this.__propertyCache__ = {};\n  var object = this; // ----------------------------------------------------------------------\n\n  this.unwrapQObject = function (response) {\n    if (response instanceof Array) {\n      // support list of objects\n      var ret = new Array(response.length);\n\n      for (var i = 0; i < response.length; ++i) {\n        ret[i] = object.unwrapQObject(response[i]);\n      }\n\n      return ret;\n    }\n\n    if (!response || !response[\"__QObject*__\"] || response.id === undefined) {\n      return response;\n    }\n\n    var objectId = response.id;\n    if (webChannel.objects[objectId]) return webChannel.objects[objectId];\n\n    if (!response.data) {\n      console.error(\"Cannot unwrap unknown QObject \" + objectId + \" without data.\");\n      return;\n    }\n\n    var qObject = new QObject(objectId, response.data, webChannel);\n    qObject.destroyed.connect(function () {\n      if (webChannel.objects[objectId] === qObject) {\n        delete webChannel.objects[objectId]; // reset the now deleted QObject to an empty {} object\n        // just assigning {} though would not have the desired effect, but the\n        // below also ensures all external references will see the empty map\n        // NOTE: this detour is necessary to workaround QTBUG-40021\n\n        var propertyNames = [];\n\n        for (var propertyName in qObject) {\n          propertyNames.push(propertyName);\n        }\n\n        for (var idx in propertyNames) {\n          delete qObject[propertyNames[idx]];\n        }\n      }\n    }); // here we are already initialized, and thus must directly unwrap the properties\n\n    qObject.unwrapProperties();\n    return qObject;\n  };\n\n  this.unwrapProperties = function () {\n    for (var propertyIdx in object.__propertyCache__) {\n      object.__propertyCache__[propertyIdx] = object.unwrapQObject(object.__propertyCache__[propertyIdx]);\n    }\n  };\n\n  function addSignal(signalData, isPropertyNotifySignal) {\n    var signalName = signalData[0];\n    var signalIndex = signalData[1];\n    object[signalName] = {\n      connect: function connect(callback) {\n        if (typeof callback !== \"function\") {\n          console.error(\"Bad callback given to connect to signal \" + signalName);\n          return;\n        }\n\n        object.__objectSignals__[signalIndex] = object.__objectSignals__[signalIndex] || [];\n\n        object.__objectSignals__[signalIndex].push(callback);\n\n        if (!isPropertyNotifySignal && signalName !== \"destroyed\") {\n          // only required for \"pure\" signals, handled separately for properties in propertyUpdate\n          // also note that we always get notified about the destroyed signal\n          webChannel.exec({\n            type: QWebChannelMessageTypes.connectToSignal,\n            object: object.__id__,\n            signal: signalIndex\n          });\n        }\n      },\n      disconnect: function disconnect(callback) {\n        if (typeof callback !== \"function\") {\n          console.error(\"Bad callback given to disconnect from signal \" + signalName);\n          return;\n        }\n\n        object.__objectSignals__[signalIndex] = object.__objectSignals__[signalIndex] || [];\n\n        var idx = object.__objectSignals__[signalIndex].indexOf(callback);\n\n        if (idx === -1) {\n          console.error(\"Cannot find connection of signal \" + signalName + \" to \" + callback.name);\n          return;\n        }\n\n        object.__objectSignals__[signalIndex].splice(idx, 1);\n\n        if (!isPropertyNotifySignal && object.__objectSignals__[signalIndex].length === 0) {\n          // only required for \"pure\" signals, handled separately for properties in propertyUpdate\n          webChannel.exec({\n            type: QWebChannelMessageTypes.disconnectFromSignal,\n            object: object.__id__,\n            signal: signalIndex\n          });\n        }\n      }\n    };\n  }\n  /**\r\n   * Invokes all callbacks for the given signalname. Also works for property notify callbacks.\r\n   */\n\n\n  function invokeSignalCallbacks(signalName, signalArgs) {\n    var connections = object.__objectSignals__[signalName];\n\n    if (connections) {\n      connections.forEach(function (callback) {\n        callback.apply(callback, signalArgs);\n      });\n    }\n  }\n\n  this.propertyUpdate = function (signals, propertyMap) {\n    // update property cache\n    for (var propertyIndex in propertyMap) {\n      var propertyValue = propertyMap[propertyIndex];\n      object.__propertyCache__[propertyIndex] = propertyValue;\n    }\n\n    for (var signalName in signals) {\n      // Invoke all callbacks, as signalEmitted() does not. This ensures the\n      // property cache is updated before the callbacks are invoked.\n      invokeSignalCallbacks(signalName, signals[signalName]);\n    }\n  };\n\n  this.signalEmitted = function (signalName, signalArgs) {\n    invokeSignalCallbacks(signalName, this.unwrapQObject(signalArgs));\n  };\n\n  function addMethod(methodData) {\n    var methodName = methodData[0];\n    var methodIdx = methodData[1];\n\n    object[methodName] = function () {\n      var args = [];\n      var callback;\n\n      for (var i = 0; i < arguments.length; ++i) {\n        var argument = arguments[i];\n        if (typeof argument === \"function\") callback = argument;else if (argument instanceof QObject && webChannel.objects[argument.__id__] !== undefined) args.push({\n          \"id\": argument.__id__\n        });else args.push(argument);\n      }\n\n      webChannel.exec({\n        \"type\": QWebChannelMessageTypes.invokeMethod,\n        \"object\": object.__id__,\n        \"method\": methodIdx,\n        \"args\": args\n      }, function (response) {\n        if (response !== undefined) {\n          var result = object.unwrapQObject(response);\n\n          if (callback) {\n            callback(result);\n          }\n        }\n      });\n    };\n  }\n\n  function bindGetterSetter(propertyInfo) {\n    var propertyIndex = propertyInfo[0];\n    var propertyName = propertyInfo[1];\n    var notifySignalData = propertyInfo[2]; // initialize property cache with current value\n    // NOTE: if this is an object, it is not directly unwrapped as it might\n    // reference other QObject that we do not know yet\n\n    object.__propertyCache__[propertyIndex] = propertyInfo[3];\n\n    if (notifySignalData) {\n      if (notifySignalData[0] === 1) {\n        // signal name is optimized away, reconstruct the actual name\n        notifySignalData[0] = propertyName + \"Changed\";\n      }\n\n      addSignal(notifySignalData, true);\n    }\n\n    Object.defineProperty(object, propertyName, {\n      configurable: true,\n      get: function get() {\n        var propertyValue = object.__propertyCache__[propertyIndex];\n\n        if (propertyValue === undefined) {\n          // This shouldn't happen\n          console.warn(\"Undefined value in property cache for property \\\"\" + propertyName + \"\\\" in object \" + object.__id__);\n        }\n\n        return propertyValue;\n      },\n      set: function set(value) {\n        if (value === undefined) {\n          console.warn(\"Property setter for \" + propertyName + \" called with undefined value!\");\n          return;\n        }\n\n        object.__propertyCache__[propertyIndex] = value;\n        var valueToSend = value;\n        if (valueToSend instanceof QObject && webChannel.objects[valueToSend.__id__] !== undefined) valueToSend = {\n          \"id\": valueToSend.__id__\n        };\n        webChannel.exec({\n          \"type\": QWebChannelMessageTypes.setProperty,\n          \"object\": object.__id__,\n          \"property\": propertyIndex,\n          \"value\": valueToSend\n        });\n      }\n    });\n  } // ----------------------------------------------------------------------\n\n\n  data.methods.forEach(addMethod);\n  data.properties.forEach(bindGetterSetter);\n  data.signals.forEach(function (signal) {\n    addSignal(signal, false);\n  });\n\n  for (var name in data.enums) {\n    object[name] = data.enums[name];\n  }\n} //required for use with nodejs\n\n\nif (( false ? undefined : _typeof(module)) === 'object') {\n  module.exports = {\n    QWebChannel: QWebChannel\n  };\n}\n/* WEBPACK VAR INJECTION */}.call(this, __webpack_require__(/*! ./../../../node_modules/_webpack@4.46.0@webpack/buildin/module.js */ \"./node_modules/_webpack@4.46.0@webpack/buildin/module.js\")(module)))\n\n//# sourceURL=webpack:///./src/common/js/qwebchannel.js?");

/***/ }),

/***/ "./src/main.js":
/*!*********************!*\
  !*** ./src/main.js ***!
  \*********************/
/*! no exports provided */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_array_iterator_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ./node_modules/_core-js@3.18.3@core-js/modules/es.array.iterator.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.array.iterator.js\");\n/* harmony import */ var _home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_array_iterator_js__WEBPACK_IMPORTED_MODULE_0___default = /*#__PURE__*/__webpack_require__.n(_home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_array_iterator_js__WEBPACK_IMPORTED_MODULE_0__);\n/* harmony import */ var _home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_promise_js__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! ./node_modules/_core-js@3.18.3@core-js/modules/es.promise.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.promise.js\");\n/* harmony import */ var _home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_promise_js__WEBPACK_IMPORTED_MODULE_1___default = /*#__PURE__*/__webpack_require__.n(_home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_promise_js__WEBPACK_IMPORTED_MODULE_1__);\n/* harmony import */ var _home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_object_assign_js__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! ./node_modules/_core-js@3.18.3@core-js/modules/es.object.assign.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.object.assign.js\");\n/* harmony import */ var _home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_object_assign_js__WEBPACK_IMPORTED_MODULE_2___default = /*#__PURE__*/__webpack_require__.n(_home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_object_assign_js__WEBPACK_IMPORTED_MODULE_2__);\n/* harmony import */ var _home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_promise_finally_js__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! ./node_modules/_core-js@3.18.3@core-js/modules/es.promise.finally.js */ \"./node_modules/_core-js@3.18.3@core-js/modules/es.promise.finally.js\");\n/* harmony import */ var _home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_promise_finally_js__WEBPACK_IMPORTED_MODULE_3___default = /*#__PURE__*/__webpack_require__.n(_home_developer_project_ishell_xterm_ishell_node_modules_core_js_3_18_3_core_js_modules_es_promise_finally_js__WEBPACK_IMPORTED_MODULE_3__);\n/* harmony import */ var vue__WEBPACK_IMPORTED_MODULE_4__ = __webpack_require__(/*! vue */ \"./node_modules/_vue@2.6.14@vue/dist/vue.runtime.esm.js\");\n/* harmony import */ var _App_vue__WEBPACK_IMPORTED_MODULE_5__ = __webpack_require__(/*! ./App.vue */ \"./src/App.vue\");\n\n\n\n\n\n\nvue__WEBPACK_IMPORTED_MODULE_4__[\"default\"].config.productionTip = false;\nnew vue__WEBPACK_IMPORTED_MODULE_4__[\"default\"]({\n  render: function render(h) {\n    return h(_App_vue__WEBPACK_IMPORTED_MODULE_5__[\"default\"]);\n  }\n}).$mount('#app');\n\n//# sourceURL=webpack:///./src/main.js?");

/***/ }),

/***/ 0:
/*!***************************!*\
  !*** multi ./src/main.js ***!
  \***************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

eval("module.exports = __webpack_require__(/*! ./src/main.js */\"./src/main.js\");\n\n\n//# sourceURL=webpack:///multi_./src/main.js?");

/***/ })

/******/ });