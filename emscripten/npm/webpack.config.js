const path = require('path');

module.exports = {
    mode: 'production',
    entry: './src/index.js',
    output: {
        filename: 'index.js',
        path: path.resolve(__dirname, 'dist'),
        libraryTarget: 'umd', // commonjs2, umd,
    },
    module: {
        rules: [
            {
                test: /\.js?$/,
                exclude: /(node_modules|VerovioEmscriptenModule)/,
                use: 'babel-loader',
            },
        ],
    },
    resolve: {
        extensions: ['.js'],
    },
    node: {
        fs: 'empty',
    },
};
