# bashrc sample


## Install

1. backup original files
```
cd ~
cp .bashrc .bashrc.org
cp .vimrc  .vimrc.org
```

2. Install
```
git clone https://github.com/nomura-kei/bashrc.git
cp bashrc/.bashrc .
cp -r bashrc/.bashrc.d .
cp bashrc/.vimrc .
cp -r bashrc/.vim .
cp bashrc/.config .
```

3. vim setup
Start vim and run ":PlugInstall".

