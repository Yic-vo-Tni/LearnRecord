## some init commands about git
```git init```
create a .git folder 

```git add .```
add files, could specify a folder

```git commit -m "xx"```
put it in the buffer

```git branch -M main```
rename the master, and can put in main branch

```git remote add "x_rename_x" "x_address_x"```
rename the address, Convenient memory

```git push -u "x_rename_x" main```
push int gitHub

## Problems in the use process
1. ```error: fail to push some refs to " "```

    原因：对GitHub里某个文件进行了在线编辑，没有同步到本地仓库

    解决：```git pull -rebase "x_rename_x" main```

    同步完成后即可进行push

2. clion git bug
    ```error: unable to read askpass response from '...\intellij-git-askpass-local.sh'```
    解决：修改config文件 ```https://userName:password@github.com/[username]/project.git```
    其中GitHub要求使用token，而非密码
    则只需将URL中的密码换位token ```https://token@github.com/[username]/project.git```

