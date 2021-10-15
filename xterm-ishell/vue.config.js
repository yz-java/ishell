const webpack = require("webpack");
module.exports = {
    publicPath: process.env.publicPath,
    lintOnSave: false,
    configureWebpack: {
        //支持jquery
        plugins: [
            new webpack.ProvidePlugin({
                $:"jquery",
                jQuery:"jquery",
                "windows.jQuery":"jquery"
            })
        ]
    },
};