# Contribution guidelines

This document sets out the methods and practices for contributing to the Verovio project.

## Contributor License Agreement

In order to contribute the the project you need to sign and send us a [Contributor License Agreement](http://docs.rism-ch.org/verovio-cla.pdf) (CLA). However, following a generally accepted practice, very small contributions (below 20 lines of code) will be accepted without signing the CLA.

## How to contribute

Adapted from the [Music Encoding Initiative Guidelines](https://raw.githubusercontent.com/music-encoding/music-encoding/master/CONTRIBUTING.md).

Please use the "Pull Request" mechanism for proposing contributions to the code base. A brief overview of this process:

 1. Fork the `rism-ch/verovio` repository into your own account.
 2. Create a dedicated branch from the `verovio:develop` branch for your developments on your repository. For example, you might create a `verovio:develop-noteheads` branch if you're proposing a developments for the note heads.
 3. Make your changes. You can commit to your branch as many times as you like.
 4. When you're ready to provide your changes "upstream," you can open a pull request to the `rism-ch/verovio:develop` branch. **NB:** You will want to make sure you're proposing a merge to `develop` and not `master`.
 5. We will then review your submission. If you are asked to make changes, you can push these changes to your original branch and the pull request will be automatically updated.
 6. Once the changes have gone through our review process, we will merge your changes into our repository. You may then delete your dedicated branch.
 
For proposing changes to the `verovio:gh-pages` branch of Verovio (e.g., its website), follow the same workflow but make the pull request to the `rism-ch/verovio:gh-pages-develop` branch.

For updating the tutorial fork the submodule [verovio-tutorial](https://github.com/rism-ch/verovio-tutorial), make a PR there (on its main branch `verovio-tutorial:gh-pages`) and wait for a merge. Afterwards create a pull request in the parent module (i.e. `verovio:gh-pages`) to point to the latest commit of the submodule by doing the following: 

- checkout a dedicated branch from the `verovio:gh-pages` branch for your changes (e.g. `verovio:gh-pages-update`)

- change to the directory of the tutorials submodule
    ```
    cd gh-tutorial
    ```

    - checkout the main branch of the submodule (it is called `gh-pages`, too)
        ```
        git checkout gh-pages
        ```
    
    - pull latest changes of remote submodule & check if it really has the latest commit
        ```
        git pull
        git log -1 (should show commit hash of latest commit in submodule repository)
        ```
    
- change back to the project root directory (Verovio main folder)
    ```
    cd ..
    ```

    - check the status to see that the submodule is now in the state you want, then commit with a concise commit message (make sure to push your changes to add them to your pull request)
        ```
        git status   # should declare: modified gh-tutorial
        git commit -am "Pulled down update to tutorial submodule"
        ```