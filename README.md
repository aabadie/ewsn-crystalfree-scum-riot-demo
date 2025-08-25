## Demo applications for the RIOT Port on SCuM

This repositories contains 2 [RIOT](https://riot-os.org) applications that can
be used with the [SCuM chip](https://crystalfree.org):

- The [crypto_aesctr](./crypto_aesctr) application implements 2 basic shell
  commands that can be used to encrypt and decrypt text messages.
- The [ai_emlearn](./ai_emlearn) application can infer hand-written digits
  contains in 2D images sent over UART. The AI engine uses
  [Emlearn](https://github.com/emlearn/emlearn) and the model is built using a
  Random Forest from [Scikit-learn](https://scikit-learn.org).

The RIOT port is based on the [SCuM SDK](https://github.com/PisterLab/scum-sdk).
You also need the
[scum-programmer Python package](https://pypi.org/project/scum-programmer/)
installed.
